//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: C
*
* This file contains all methods relevant for time keeping and time calculation for 
* the PiOS
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//TODO: documentation

#include <time.h>
#include <peripherals.h>

uint32_t get_time_cycle()
{
  return *(volatile uint32_t*)(IO_BASE + SYSTEM_TIMER + SYS_TIME_LOW);
}

/*
* A function to wait.
*
* Parameters:
*   uint32_t time: the amount of time to wait.
*/
void wait(uint32_t time) __attribute__((optimize("-O0")));
void wait(uint32_t time)
{
  volatile uint32_t *s_timer = 
    (volatile uint32_t*)(IO_BASE + SYSTEM_TIMER + SYS_TIME_LOW);
  uint32_t end = *s_timer + time;

  while (*s_timer < end){}
}