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

//TODO: documentation

#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <peripherals.h>

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