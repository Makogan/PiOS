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

#include "mailbox.h"

//########################################################################################

//========================================================================================
/*
* Globals:
*/
//========================================================================================

volatile struct Message led_message =
{
  .messageSize = sizeof(struct Message),
  .requestCode =0,
  .tagID = 0x00038041,
  .bufferSize = 8,
  .requestSize =0,
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
void write_to_mailbox(uint32_t message, uint32_t channel)
{
  uint32_t status;

  do
    status = *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x18);
  while((status & 0x80000000));

  *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x20) = ((uint32_t)(message) & 0xfffffff0) | (uint32_t)(8);
}

/*
* Function to read a message from the mailbox
* 
* Parameters:
*   uint32_t channel: the channel numer from which the message is read
*
* Returns:
*   uint32_t: the content of the mailboxes response register for the given channel
*/
uint32_t read_from_mailbox(uint32_t channel)
{
  uint32_t status;

  do
    // Get value of address of mailbox status register into status
    status = *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x18);
  // If the empty bit is set repeat until mailbox not empty
  while((status & 0x40000000)); 

  uint32_t response;

  do
    // Get value of mailbox read register
    response = (*(uint32_t *)(MAIL_BASE + IO_BASE));
  // If the response channel is not that of the requested channel repeat
  while((response & 0xF) != channel);

  return response;
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
  led_message.requestCode = 0;
  led_message.requestSize = 0;
  led_message.pinNum = 130;
  led_message.end = 0;

  write_to_mailbox(((uint32_t) &led_message), 8);

  read_from_mailbox(8);
}
//########################################################################################