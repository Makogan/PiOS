#include <stdint.h>

#define ON 1
#define OFF 0

#define REGISTERS_BASE 0x3F000000
#define MAIL_BASE 0xB880
#define MAIL_FULL 0x80000000
#define MAIL_EMPTY 0x40000000

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

void write_to_mailbox(uint32_t message);
uint32_t read_from_mailbox(uint32_t channel);

void set_LED(int value);

#ifdef __cplusplus
  }
#endif
