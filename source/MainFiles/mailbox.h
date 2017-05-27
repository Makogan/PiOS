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

#include <stdint.h>

#define ON 1     // ACT LED ON
#define OFF 0    // ACT LED OFF


#define IO_BASE 0x3F000000    // Base IO address for Raspberry Pi™ model 3
#define MAIL_BASE 0xB880      // Base address for the mailbox
#define MAIL_FULL 0x80000000  // 32 bit value, MSB is 1, used to check state of MSB 
                              // (full bit in the mailbox status register)
#define MAIL_EMPTY 0x40000000 // 32 bit value, 31st is 1, used to check state of 31st bit
                              // (empty bit in the mailbox status register)
 
struct Message
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

#ifdef __cplusplus
  extern "C"
  {
#endif

void write_to_mailbox(uint32_t message, uint32_t channel);
uint32_t read_from_mailbox(uint32_t channel);

void set_LED(int value);

#ifdef __cplusplus
  }
#endif
