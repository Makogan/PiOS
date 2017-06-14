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

#ifdef __cplusplus
extern "C"
{
#endif

void wait(uint32_t time);
uint32_t get_time_cycle();

#ifdef __cplusplus
}
#endif

#endif