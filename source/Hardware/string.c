//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: C
*
* This file is the PiOS implementation of the print function, it has all the implemented
* methods for string manipulation and basic I/O
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//TODO: documentation

#include <string.h>
#include <fonts.h>

#define BIT(val, bit) val & (1 << bit)

#define SIZE 10
#define CHAR_IMAGE_SIZE CHAR_BITS*CHAR_BITS*SIZE*SIZE
uint32_t char_buffer[CHAR_IMAGE_SIZE];

void init_char_image(const char* charMap, uint32_t size, uint32_t* drawnChar)
{
  for(uint32_t i=0; i<CHAR_BITS*size; i++)
  {
    uint32_t line = charMap[i/size];
    for(uint32_t j=0; j<CHAR_BITS*size; j++)
    {
      drawnChar[i*CHAR_BITS*size+j] = (BIT(line, 0))*WHITE_32;
      if(j%size == 0 && j>0)
        line = line >> 1;
    }
  }
}

void drawChar(uint32_t *characterImage, uint32_t size, 
  uint32_t x_offset, uint32_t y_offset)
{
  uint32_t scaling = CHAR_BITS*size;
  x_offset *= scaling;
  y_offset *= scaling;
  for(uint32_t i=0; i<scaling; i++)
  {
    for(uint32_t j=0; j<scaling; j++)
    {
      *(volatile uint32_t *)((main_monitor.fb_ptr & ~BUS_MASK) 
        + ((i+y_offset)*main_monitor.virtual_width + (j+x_offset))
        *main_monitor.color_depth/CHAR_BITS) = 
        characterImage[i*scaling+j];
    }
  }
}

void print(char* string)
{
  uint32_t size = SIZE, x=0, y=0;

  for(uint32_t i=0; string[i] != '\0'; i++)
  { 
    if(x >= main_monitor.virtual_width/(size*CHAR_BITS))
    {
      x = 0;
      y++;
    }

    if(string[i] == '\n')
    {
      x=0;
      y++;
    }
    
    else
    {
      const char *currentChar = basic_font[(uint32_t)string[i]];
      init_char_image(currentChar, size, char_buffer);
      drawChar(char_buffer, size, x, y);
      x++;
    }
  }
} 