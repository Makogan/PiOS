//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: C++
*
* This file contains all functions needed to communicate with the Raspberry Pi™ ARM
* video core.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
* Includes and macro definitions:
*/
//========================================================================================

#include <mailbox.h>
#include <fonts.h>
#include <colors.h>

#define BIT(val, bit) val & (1 << bit)
//########################################################################################

//========================================================================================
/*
* Globals:
*/
//========================================================================================

volatile Mail_Message_LED led_message =
{
  .messageSize = sizeof(struct Mail_Message_LED),
  .request_response_code =0,
  .tagID = SET_GPIO_STATE,
  .bufferSize = 8,
  .requestSize = 0,
  .pinNum = 130,
  .on_off_switch = 1,
  .end = 0,
};

uint32_t mailbox_message[22] __attribute__ ((aligned (16)));
uint32_t index;

//########################################################################################

//========================================================================================
/*
* Function definitions:
*/
//========================================================================================

/*
* Function to write a message to the mailbox
*
* Parameters:
*   uint32_t message: the address to the message struct containing the message information
*   uint32_t channel: the channel numer to which the message is sent
*/
void write_to_mailbox(uint32_t message, Channel channel)
{
  uint32_t status;

  do
    status = *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x18);
  while((status & MAIL_FULL));

  *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x20) = (((uint32_t)(message) & ~0xF) | (uint32_t)(channel));
}

/*
* Function to read a message from the mailbox
*
* Parameters:
*   Channel channel: the channel number from which the message is read
*
* Returns:
*   the content of the mailboxes response register for the given channel
*/
uint32_t read_from_mailbox(Channel channel)
{
  uint32_t status;

  do
    // Get value of address of mailbox status register into status
    status = *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x18);
  // If the empty bit is set repeat until mailbox not empty
  while((status & MAIL_EMPTY));

  uint32_t response;

  do
    // Get value of mailbox read register
    response = (*(uint32_t *)(MAIL_BASE + IO_BASE));
  // If the response channel is not that of the requested channel repeat
  while((response & 0xF) != channel);

  return response & ~0xF;
}

/*
* Function to turn the ACT LED on or off
*
* Parameters:
*   int value: one of ON or OFF
*/
void set_LED(int value)
{
  // Set the default message values
  led_message.on_off_switch = (uint32_t) value;
  led_message.request_response_code = 0;
  led_message.requestSize = 0;
  led_message.pinNum = 130;
  led_message.end = END;

  write_to_mailbox(((uint32_t) &led_message), PTAG_ARM_TO_VC);
  read_from_mailbox(PTAG_ARM_TO_VC);
}

void wait(uint32_t time)
{
  while (time > 0)
    time--;
}
void blink()
{
  set_LED(ON);
  wait(0xF0000);

  set_LED(OFF);
  wait(0xF0000);
}

void drawChar(/*const char* charMap, uint32_t size, uint32_t* drawnChar*/)
{
  /*for(uint32_t i=0; i<CHAR_BITS*size; i++)
  {
    uint32_t line = charMap[i/size];
    for(uint32_t j=0; j<CHAR_BITS*size; j++)
    {
      drawnChar[i*CHAR_BITS*size+j] = (BIT(line, 0))*WHITE_32;
      if(j%size == 0 && j>0)
        line = line >> 1;
    }
  }*/
} 

void set_init_display_message(Display_info *disp)
{
  index = 0;
  #define MESSAGE_SIZE 0
  mailbox_message[index++] = 22*sizeof(uint32_t);
  #define FB_RESPONSE 1
  mailbox_message[index++] = 0;

  mailbox_message[index++] = (uint32_t) SET_PHYSICAL_WIDTH_HEIGHT;
  mailbox_message[index++] = 8;
  mailbox_message[index++] = 8;
  mailbox_message[index++] = disp->physical_width;
  mailbox_message[index++] = disp->physical_height;

  mailbox_message[index++] = (uint32_t) SET_VIRTUAL_WIDTH_HEIGHT;
  mailbox_message[index++] = 8;
  mailbox_message[index++] = 8;
  #define FB_VIRTUAL_WIDTH 10
  mailbox_message[index++] = disp->virtual_width;
  #define FB_VIRTUAL_HEIGHT 11
  mailbox_message[index++] = disp->virtual_height;

  mailbox_message[index++] = (uint32_t) SET_DEPTH;
  mailbox_message[index++] = 4;
  mailbox_message[index++] = 4;
  #define FB_DEPTH 15
  mailbox_message[index++] = disp->color_depth;

  mailbox_message[index++] = (uint32_t) ALLOCATE;
  mailbox_message[index++] = 8;
  mailbox_message[index++] = 8;
  #define FB_PTR 19
  mailbox_message[index++] = 16;
  #define FB_SIZE 20
  mailbox_message[index++] = 0;

  mailbox_message[index++] = END;
}

void init_display()
{
  Display_info disp = {1360, 768, 1360, 768, 32};

  set_init_display_message(&disp);

  write_to_mailbox((uint32_t) &mailbox_message | BUS_MASK, (Channel)(PTAG_ARM_TO_VC));
  read_from_mailbox(PTAG_ARM_TO_VC);

  while(mailbox_message[FB_RESPONSE] == RESPONSE_ERROR)
  {
   // blink();
  }

  /*for(uint32_t i=0; i < mailbox_message[FB_SIZE]; i+=mailbox_message[FB_DEPTH]/CHAR_BITS)
  {
    *(volatile uint32_t *)((mailbox_message[FB_PTR] & ~BUS_MASK) + i) = BLUE_32;
  }*/

  uint32_t size = 11;

  size = 11;
  for(uint32_t i=0; i<CHAR_BITS*size; i++)
  {
    for(uint32_t j=0; j<CHAR_BITS*size; j++)
    {
      *(volatile uint32_t *)((mailbox_message[FB_PTR] & ~BUS_MASK) 
        + (i*mailbox_message[FB_VIRTUAL_WIDTH] + j)*mailbox_message[FB_DEPTH]/CHAR_BITS) = WHITE_32;//charTest[i*CHAR_BITS*size+j];
    }
  }

  const char *test = basic_font['A'];
  uint32_t charTest[CHAR_BITS*CHAR_BITS*size*size];
  drawChar(/*test, size, charTest*/);

  size = 10;
  for(uint32_t i=0; i<CHAR_BITS*size; i++)
  {
    for(uint32_t j=0; j<CHAR_BITS*size; j++)
    {
      *(volatile uint32_t *)((mailbox_message[FB_PTR] & ~BUS_MASK) 
        + (i*mailbox_message[FB_VIRTUAL_WIDTH] + j)*mailbox_message[FB_DEPTH]/CHAR_BITS) = CYAN_32;//charTest[i*CHAR_BITS*size+j];
    }
  }
}


//########################################################################################
