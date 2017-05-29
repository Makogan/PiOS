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
  uint32_t requestCode;
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
struct Mail_Message_FrameBuffer
{
  uint32_t disp_width;
  uint32_t disp_height;
  uint32_t buff_width;
  uint32_t buff_height;

  uint32_t pitch; // request 0 response = num of bytes between each row of the fb ?????????
  uint32_t bits_per_pixel;
  uint32_t x_pos; // x offset of virtual fb
  uint32_t y_pos; // y offset of requested fb
  uint32_t buff_address // request set to 0 response 0 if fails or address of buffer allocated by vc;
  uint32_t buff_size; // request set to 0, response size of buffer alocated by vc

  uint32_t end;
};

#ifdef __cplusplus
  extern "C"
  {
#endif

void write_to_mailbox(uint32_t message, uint32_t channel);
uint32_t read_from_mailbox(Channel channel);

void set_LED(int value);

#ifdef __cplusplus
  }
#endif

#endif