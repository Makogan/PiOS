//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: Header
*
* Associated files: font_info.c, basic_font.c
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//TODO: documentation
#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#define CHAR_BITS 8

extern uint32_t font_size;
extern const char (*selected_font)[CHAR_BITS];
extern const char basic_font[128][CHAR_BITS];

#endif
