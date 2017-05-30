//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero Nasir Osman
*
*
* File type: Header
*
* Associated files: mailbox.cpp
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>
#include <peripherals.h>

#define ON 1     // ACT LED ON
#define OFF 0    // ACT LED OFF

#define MAIL_FULL 0x80000000  // 32 bit value, MSB is 1, used to check state of MSB
                              // (full bit in the mailbox status register)
#define MAIL_EMPTY 0x40000000 // 32 bit value, 31st is 1, used to check state of 31st
                              // bit (empty bit in the mailbox status register)

#define END 0
#define ALLOCATE_BUFFER 0x00040001
#define BLANK_SCREEN 0x00040002
#define GET_PHYS_DISP 0x00040003
#define TEST_PHYS_DISP 0x00044003
#define SET_PHYS_DISP 0x00048003
#define SET_VIRT_FB 0x00048004
#define SET_COLOR_DEPTH 0x00048005


typedef enum
{
  POWER_MANAGEMENT = 0,
  FRAMEBUFFER,
  VIRTUAL_UART,
  VCHIQ,
  LEDS,
  BUTTONS,
  TOUCH_SCREEN,
  UNDEFINED,
  PTAG_ARM_TO_VC,
  PTAG_VC_TO_ARM

}Channel;

/*
* This is the structure definition for a led message
*/
struct Mail_Message_LED
{
  uint32_t messageSize;
  uint32_t request_response_code;
  uint32_t tagID;
  uint32_t bufferSize;
  uint32_t requestSize;
  uint32_t pinNum;
  uint32_t on_off_switch;
  uint32_t end;
};

/*
* This is the structure definition for a frame buffer message
*/
struct Mail_Message_FB
{
  uint32_t messageSize;
  uint32_t request_response_code;

  uint32_t tagID;
  uint32_t bufferSize;
  uint32_t requestSize;

  int response_request1;
  int response_request2;

  uint32_t end;
};

#ifdef __cplusplus
  extern "C"
  {
#endif

void write_to_mailbox(uint32_t message, uint32_t channel);
uint32_t read_from_mailbox(Channel channel);

void set_LED(int value);
void init_display();
int test();

#ifdef __cplusplus
  }
#endif

#endif
