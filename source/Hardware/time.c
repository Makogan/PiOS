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

//========================================================================================
/*
* Includes and macro definitions:
*/
//========================================================================================

#include <time.h>

//########################################################################################

//========================================================================================
/*
* Global symbols:
*/
//========================================================================================

// initialize the system timer struct to the correct memory address
volatile system_timer_t *system_timer = (volatile system_timer_t*)(IO_BASE+SYSTEM_TIMER);

//########################################################################################

//========================================================================================
/*
* Function definitions:
*/
//========================================================================================

/*
* A simple function to set the timer irq to be called
*
* Parameters:
*   uin32_t time: The number of cycles at which the interrupt will be calle counting from
*                the current cycle
*/
void set_time_irq(uint32_t time)
{
  system_timer->compare_1 = system_timer->counter_low + time;
}

/*
* A function to get the current number of cycles in the low counter of the system timer
*
* Returns:
*     uint32_t: the current number of cycles of the low counter of the system timer
*/
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

//########################################################################################