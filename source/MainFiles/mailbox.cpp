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

  *(volatile uint32_t *)(MAIL_BASE + IO_BASE + 0x20) = ((uint32_t)(message) /*<< 4)*/ | (uint32_t)(channel));
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

void init_display()
{
  Mail_Message_FB message;

  message.messageSize = sizeof(Mail_Message_FB);
  message.request_response_code = 0;
  message.tagID = SET_PHYSICAL_WIDTH_HEIGHT;
  message.bufferSize = 8;
  message.requestSize = 8;
  message.response_request1 = 1024;
  message.response_request2 = 768;
  message.end = END;

  write_to_mailbox((uint32_t) &message, PTAG_ARM_TO_VC);

  message.request_response_code = 0;
  message.tagID = SET_VIRTUAL_WIDTH_HEIGHT;
  message.bufferSize = 8;
  message.requestSize = 8;
  message.response_request1 = 1024;
  message.response_request2 = 768;
  message.end = END;

  write_to_mailbox((uint32_t) &message, PTAG_ARM_TO_VC);

  message.request_response_code = 0;
  message.tagID = SET_DEPTH;
  message.bufferSize = 4;
  message.requestSize = 4;
  message.response_request1 = 16;
  message.response_request2 = 0;
  message.end = END;

  write_to_mailbox((uint32_t) &message, PTAG_ARM_TO_VC);

  message.request_response_code = 0;
  message.tagID = ALLOCATE;
  message.bufferSize = 8;
  message.requestSize = 8;
  message.response_request1 = 0;
  message.response_request2 = 0;
  message.end = END;

  write_to_mailbox((uint32_t) &message, PTAG_ARM_TO_VC);

  for(uint i; i<message.response_request2; i++)
  {
    *(volatile uint32_t)(message.response_request1 + i*2) = 0;
  }
}

int test()
{
  Mail_Message_FB message;

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
