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

#include "timePiOS.h"

typedef struct
{
  uint32_t control_status;
  uint32_t counter_low;
  uint32_t counter_high;
  uint32_t compare_0;
  uint32_t compare_1;
  uint32_t compare_2;
  uint32_t compare_3;
}system_timer_t;

volatile system_timer_t *system_timer = (volatile system_timer_t*)(IO_BASE+SYSTEM_TIMER);

void set_time_irq(uint32_t time)
{
  system_timer->compare_1 = system_timer->counter_low + time;
}

void clear_time_irq()
{
    system_timer->control_status = (1 << 1);
}

uint32_t get_time_cycle()
{
  return system_timer->counter_low;
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
  uint32_t end = system_timer->counter_low + time;

  while (system_timer->counter_low < end){}
}