#include "mailbox.h"

void wait(uint32_t time);

/** Main function - we'll never return from here */
int kernel_main(void)
{

  while(1)
  {
    set_LED(ON);

    wait(0xF0000);

    set_LED(OFF);

    wait(0xF0000);
  }

}

void wait(uint32_t time)
{
  while (time > 0) {
    time--;
  }
}
