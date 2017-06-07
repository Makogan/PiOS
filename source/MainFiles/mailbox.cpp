//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero Nasir Osman
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

struct temp
{
  volatile uint32_t size;
  volatile uint32_t request;

  volatile uint32_t tag1;
  volatile uint32_t buff_size1;
  volatile uint32_t val_length1;
  volatile uint32_t widthP;
  volatile uint32_t heightP;

  volatile uint32_t tag2;
  volatile uint32_t buff_size2;
  volatile uint32_t val_length2;
  volatile uint32_t widthV;
  volatile uint32_t heightV;

  volatile uint32_t tag3;
  volatile uint32_t buff_size3;
  volatile uint32_t val_length3;
  volatile uint32_t depth;

  volatile uint32_t tag4;
  volatile uint32_t buff_size4;
  volatile uint32_t val_length4;
  volatile uint32_t fb_size;   
  volatile uint32_t fb_ptr;
 
  volatile uint32_t end;
};

volatile temp t __attribute__ ((aligned (16)))=
{
  .size = sizeof(temp),
  .request = 0,

  .tag1 = SET_PHYSICAL_WIDTH_HEIGHT,
  .buff_size1 = 8,
  .val_length1 = 8,
  .widthP = 1024,
  .heightP = 768,

  .tag2 = SET_VIRTUAL_WIDTH_HEIGHT,
  .buff_size2 = 8,
  .val_length2 = 8,
  .widthV = 1024,
  .heightV = 768,

  .tag3 = SET_DEPTH,
  .buff_size3 = 4,
  .val_length3 = 4,
  .depth = 16,

  .tag4 = ALLOCATE,
  .buff_size4 = 8,
  .val_length4 = 4,
  .fb_size = 16,
  .fb_ptr = 0,

  .end = END,
};

void wait(uint32_t time)
{
  while (time > 0)
    time--;
}

void blink()
{
  wait(0xF0000);

  set_LED(ON);

  wait(0xF0000);

  set_LED(OFF);
}

void init_display()
{
  wait(0x100000);

  write_to_mailbox((uint32_t) &t , (Channel)(PTAG_ARM_TO_VC));
  read_from_mailbox(PTAG_ARM_TO_VC);

  while(t.request != 0x80000000)
  {
    blink();
  }

  for(int i=0; i<10; i++)
  {
    *(volatile uint32_t *)((t.fb_ptr & ~0xC0000000) + i*2) = 0xFFFF;
  }
}

int test()
{
  volatile Mail_Message_FB message;

  message.messageSize = sizeof(Mail_Message_FB);
  message.request_response_code = 0;
  message.tagID = BLANK_SCREEN;
  message.bufferSize = 4;
  message.requestSize = 4;
  message.response_request1 = 1;
  message.response_request2 = 0;
  message.end = END;

  write_to_mailbox((uint32_t) &message, PTAG_ARM_TO_VC);
  read_from_mailbox(PTAG_ARM_TO_VC);
}
//########################################################################################
