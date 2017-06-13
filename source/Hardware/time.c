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

#pragma GCC optimize ("-O0")
/*
* A function to wait.
*
* Parameters:
*   uint32_t time: the amount of time to wait.
*/
void wait(uint32_t time)
{
  volatile uint32_t *s_timer = 
    (volatile uint32_t*)(IO_BASE + SYSTEM_TIMER + SYS_TIME_LOW);
  uint32_t end = *s_timer + 1000000;

  while (*s_timer < end){}
}
#pragma GCC optimize ("-O2")