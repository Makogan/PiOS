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

#define MAIL_FULL 0x80000000  // 32 bit value, MSB is 1, used to check state of MSB
                              // (full bit in the mailbox status register)
#define MAIL_EMPTY 0x40000000 // 32 bit value, 31st is 1, used to check state of 31st
                              // bit (empty bit in the mailbox status register)

#define RESPONSE_OK     0x80000000 // Same as MAIL_FULL define nonetheless for clarity
#define RESPONSE_ERROR  0x80000001 // If this is the response, the VC found an error

//########################################################################################

//========================================================================================
/*
* Global symbols:
*/
//========================================================================================

uint32_t mailbox_message[22] __attribute__ ((aligned (16)));
uint32_t index;

Display_info main_monitor;
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
  index = 1;
  mailbox_message[index++] =0, //request
  mailbox_message[index++] = (uint32_t) SET_GPIO_STATE, //tag
  mailbox_message[index++] = 8, //request size
  mailbox_message[index++] = 0, // response size
  mailbox_message[index++] = 130, //pin num
  mailbox_message[index++] = value, //state ON or OFF
  mailbox_message[index++] = 0, //end tag

  mailbox_message[0] = index*sizeof(uint32_t); //Total message size

  write_to_mailbox(((uint32_t) &mailbox_message | BUS_MASK), PTAG_ARM_TO_VC);
  read_from_mailbox(PTAG_ARM_TO_VC);
}

/*
* FUnction to blink the ACT LED
*/
void blink()
{
  set_LED(ON);
  wait(0xF0000);

  set_LED(OFF);
  wait(0xF0000);
}

/*
* Function to create a proper frame buffer initialization message
*
* Parameters:
*   Display_info *disp: a pointer to a Display_info structure that stores the desired
*                      information with which to initialize the frambuffer.
*/
void set_init_display_message(Display_info *disp)
{
  index = 1;
  #define FB_RESPONSE 1
  mailbox_message[index++] = 0;//request code

  mailbox_message[index++] = (uint32_t) SET_PHYSICAL_WIDTH_HEIGHT; //tag
  mailbox_message[index++] = 8;//request size
  mailbox_message[index++] = 8;//response size
  mailbox_message[index++] = disp->physical_width;//horizontal resolution of the monitor
  mailbox_message[index++] = disp->physical_height;//vertical resolution of the monitor

  mailbox_message[index++] = (uint32_t) SET_VIRTUAL_WIDTH_HEIGHT;//tag
  mailbox_message[index++] = 8;//rquest size
  mailbox_message[index++] = 8;// response size
  #define FB_VIRTUAL_WIDTH 10
  mailbox_message[index++] = disp->virtual_width;//horizontal resolution of virtual screen
  #define FB_VIRTUAL_HEIGHT 11
  mailbox_message[index++] = disp->virtual_height;//vertical resolution of virtual screen

  mailbox_message[index++] = (uint32_t) SET_DEPTH;//tag
  mailbox_message[index++] = 4;//request size
  mailbox_message[index++] = 4;//response size
  #define FB_DEPTH 15
  mailbox_message[index++] = disp->color_depth;//color depth of the frame buffer

  mailbox_message[index++] = (uint32_t) ALLOCATE;//tag
  mailbox_message[index++] = 8;//request size
  mailbox_message[index++] = 8;//response size
  #define FB_PTR 19
  mailbox_message[index++] = 16;//alignment fb ptr returned here
  #define FB_SIZE 20
  mailbox_message[index++] = 0;//fb size returned here

  mailbox_message[index++] = END;//end tag

  #define MESSAGE_SIZE 0
  mailbox_message[0] = index*sizeof(uint32_t);//size of message
}

/*
* Function to initialize the virtual screen for displaying information
*/
void init_display()
{
  //set global main_monitor values
  main_monitor = {1360, 768, 1360, 768, 32, 0, 0};

  //create an appropriate FB initialization message for the VC
  set_init_display_message(&main_monitor);

  // Send the message
  write_to_mailbox((uint32_t) &mailbox_message | BUS_MASK, (Channel)(PTAG_ARM_TO_VC));
  read_from_mailbox(PTAG_ARM_TO_VC);

  //if there was an error stop here
  while(mailbox_message[FB_RESPONSE] == RESPONSE_ERROR)
  {
    blink();
  }

  // update the main monitor info
  main_monitor.fb_ptr = mailbox_message[FB_PTR];
  main_monitor.fb_size = mailbox_message[FB_SIZE];
}
//########################################################################################
