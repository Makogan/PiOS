//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: Header
*
* Associated files: time.c
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <peripherals.h>


//System timer struct, each field is a register in the system counter peripheral
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

extern volatile system_timer_t *system_timer;

//Expose these functions to other files
#ifdef __cplusplus
extern "C"
{
#endif

void wait(uint32_t time);
void set_time_irq(uint32_t time);
void clear_time_irq();
uint32_t get_time_cycle();

#ifdef __cplusplus
}
#endif

#endif