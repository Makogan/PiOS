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
#include <fonts.h>
#include <memory_management.h>

typedef struct
{
  uint32_t x;
  uint32_t y;
}Cursor;

extern Cursor main_cursor;

void init_print(uint32_t f_size);
void print(uint32_t num);
void print(char* string);
void print(const char* string);
char* itos(uint32_t num);

#endif