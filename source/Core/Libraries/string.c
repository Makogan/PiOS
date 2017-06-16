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

#define BIT(val, bit) val & (1 << bit)

Cursor main_cursor;

void init_print(uint32_t f_size)
{
  font_size = f_size;
  selected_font = &basic_font[0];
}

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

char* itos(uint32_t num)
{
  uint32_t original_num = num;
  int digits = 0;
  do
  {
    digits++;
    num /= 10; 
  }
  while(num != 0);

  num = original_num;
  char* str = (char*)memory_alloc(sizeof(char)*(digits+1));
  str[digits--] = 0;

  while(digits>=0)
  {
    int digit = num % 10;
    str[digits--] = digit + '0';
    num /= 10;
  }

  return str;
}

void print(uint32_t num)
{
  char* str = itos(num);
  print(str);
  memory_free(str);
}

void print(const char* string)
{
  print((char*)string);
}

void print(char* string)
{
  uint32_t x = main_cursor.x;
  uint32_t y = main_cursor.y; 

  uint32_t size = font_size;
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
      uint32_t tempBuffer[font_size*font_size*CHAR_BITS*CHAR_BITS];
      const char *currentChar = (selected_font[(uint32_t)string[i]]);
      init_char_image(currentChar, size, tempBuffer);
      init_char_image(currentChar, size, char_buffer);
      drawChar(tempBuffer, size, x, y);
      x++;
    }
  }

  main_cursor.x = x;
  main_cursor.y = (y)%(main_monitor.virtual_height/(size*CHAR_BITS));
} 