//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
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

//Taken from: https://github.com/raspberrypi/linux/blob/rpi-4.9.y/include/soc/bcm2835/raspberrypi-firmware.h
typedef enum {
  END =                                    0,
  GET_FIRMWARE_REVISION =                  0x00000001,

  SET_CURSOR_INFO =                        0x00008010,
  SET_CURSOR_STATE =                       0x00008011,

  GET_BOARD_MODEL =                        0x00010001,
  GET_BOARD_REVISION =                     0x00010002,
  GET_BOARD_MAC_ADDRESS =                  0x00010003,
  GET_BOARD_SERIAL =                       0x00010004,
  GET_ARM_MEMORY =                         0x00010005,
  GET_VC_MEMORY =                          0x00010006,
  GET_CLOCKS =                             0x00010007,
  GET_POWER_STATE =                        0x00020001,
  GET_TIMING =                             0x00020002,
  SET_POWER_STATE =                        0x00028001,
  GET_CLOCK_STATE =                        0x00030001,
  GET_CLOCK_RATE =                         0x00030002,
  GET_VOLTAGE =                            0x00030003,
  GET_MAX_CLOCK_RATE =                     0x00030004,
  GET_MAX_VOLTAGE =                        0x00030005,
  GET_TEMPERATURE =                        0x00030006,
  GET_MIN_CLOCK_RATE =                     0x00030007,
  GET_MIN_VOLTAGE =                        0x00030008,
  GET_TURBO =                              0x00030009,
  GET_MAX_TEMPERATURE =                    0x0003000a,
  GET_STC =                                0x0003000b,
  ALLOCATE_MEMORY =                        0x0003000c,
  LOCK_MEMORY =                            0x0003000d,
  UNLOCK_MEMORY =                          0x0003000e,
  RELEASE_MEMORY =                         0x0003000f,
  EXECUTE_CODE =                           0x00030010,
  EXECUTE_QPU =                            0x00030011,
  SET_ENABLE_QPU =                         0x00030012,
  GET_DISPMANX_RESOURCE_MEM_HANDLE =       0x00030014,
  GET_EDID_BLOCK =                         0x00030020,
  GET_CUSTOMER_OTP =                       0x00030021,
  GET_DOMAIN_STATE =                       0x00030030,
  SET_CLOCK_STATE =                        0x00038001,
  SET_CLOCK_RATE =                         0x00038002,
  SET_VOLTAGE =                            0x00038003,
  SET_TURBO =                              0x00038009,
  SET_CUSTOMER_OTP =                       0x00038021,
  SET_DOMAIN_STATE =                       0x00038030,
  GET_GPIO_STATE =                         0x00030041,
  SET_GPIO_STATE =                         0x00038041,
  SET_SDHOST_CLOCK =                       0x00038042,
  GET_GPIO_CONFIG =                        0x00030043,
  SET_GPIO_CONFIG =                        0x00038043,

  /* Dispmanx TAGS */
  ALLOCATE =                   0x00040001,
  BLANK_SCREEN =               0x00040002,
  GET_PHYSICAL_WIDTH_HEIGHT =  0x00040003,
  GET_VIRTUAL_WIDTH_HEIGHT =   0x00040004,
  GET_DEPTH =                  0x00040005,
  GET_PIXEL_ORDER =            0x00040006,
  GET_ALPHA_MODE =             0x00040007,
  GET_PITCH =                  0x00040008,
  GET_VIRTUAL_OFFSET =         0x00040009,
  GET_OVERSCAN =               0x0004000a,
  GET_PALETTE =                0x0004000b,
  GET_TOUCHBUF =               0x0004000f,
  GET_GPIOVIRTBUF =            0x00040010,
  RELEASE =                    0x00048001,
  TEST_PHYSICAL_WIDTH_HEIGHT = 0x00044003,
  TEST_VIRTUAL_WIDTH_HEIGHT =  0x00044004,
  TEST_DEPTH =                 0x00044005,
  TEST_PIXEL_ORDER =           0x00044006,
  TEST_ALPHA_MODE =            0x00044007,
  TEST_VIRTUAL_OFFSET =        0x00044009,
  TEST_OVERSCAN =              0x0004400a,
  TEST_PALETTE =               0x0004400b,
  TEST_VSYNC =                 0x0004400e,
  SET_PHYSICAL_WIDTH_HEIGHT =  0x00048003,
  SET_VIRTUAL_WIDTH_HEIGHT =   0x00048004,
  SET_DEPTH =                  0x00048005,
  SET_PIXEL_ORDER =            0x00048006,
  SET_ALPHA_MODE =             0x00048007,
  SET_VIRTUAL_OFFSET =         0x00048009,
  SET_OVERSCAN =               0x0004800a,
  SET_PALETTE =                0x0004800b,
  SET_TOUCHBUF =               0x0004801f,
  SET_GPIOVIRTBUF =            0x00048020,
  SET_VSYNC =                  0x0004800e,
  SET_BACKLIGHT =              0x0004800f,

  VCHIQ_INIT =                 0x00048010,

  GET_COMMAND_LINE =           0x00050001,
  GET_DMA_CHANNELS =           0x00060001,
}Property_tag;


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
  Property_tag tagID;
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
  int messageSize;
  int request_response_code;

  int tagID;
  int bufferSize;
  int requestSize;

  int response_request1;
  int response_request2;

  int end;
};

#ifdef __cplusplus
  extern "C"
  {
#endif

void write_to_mailbox(uint32_t message, uint32_t channel);
uint32_t read_from_mailbox(Channel channel);

void set_LED(int value);
void init_display();
void test();

#ifdef __cplusplus
  }
#endif

#endif
