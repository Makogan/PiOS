#include <stdint.h>

#define REGISTERS_BASE 0x3F000000
#define MAIL_BASE 0xB880  // Base address for the mailbox registers
// This bit is set in the status register if there is no space to write into the mailbox
#define MAIL_FULL 0x80000000
// This bit is set in the status register if there is nothing to read from the mailbox
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

volatile struct Message m =
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

/** Main function - we'll never return from here */
int kernel_main(void)
{
  uint32_t mailbox = MAIL_BASE + REGISTERS_BASE + 0x18;
  volatile uint32_t status;

  while(1)
  {
    do
    {
      status = *(volatile uint32_t *)(mailbox);
    }
    while((status & 0x80000000));

    *(volatile uint32_t *)(MAIL_BASE + REGISTERS_BASE + 0x20) = ((uint32_t)(&m) & 0xfffffff0) | (uint32_t)(8);

    int i=0;

    while(i<0xF0000)
    {
      i++;
    }

    if(m.on_off_switch == 0)
    {
      m.on_off_switch = 1;
    }

    else
    {
      m.on_off_switch = 0;
    }

    m.requestCode = 0;
    m.requestSize = 0;
    m.pinNum = 130;
    m.end = 0;

    do
    {
      status = *(volatile uint32_t *)(mailbox);
    }
    while((status & 0x40000000));

    uint32_t temp;

    do
    {
      temp = *(uint32_t *)(MAIL_BASE + REGISTERS_BASE);
      temp = temp & 0xF;
    }
    while(temp != 8);

  }

}