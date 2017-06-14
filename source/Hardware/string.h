//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: Header
*
* Associated files: string.c
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//TODO: documentation

#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <mailbox.h>

#ifdef __cplusplus
    extern "C" {
#endif

void print(char* string);
void tempitos(uint32_t num, char *str);

#ifdef __cplusplus
    }
#endif


#endif