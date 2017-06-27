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

//========================================================================================
/*
* Includes and macro definitions:
*/
//========================================================================================

#include <string.h>

#define BIT(val, bit) val & (1 << bit)

//########################################################################################

//========================================================================================
/*
* Global symbol deffinitions:
*/
//========================================================================================

Cursor main_cursor;
uint32_t *charBuffer;

//########################################################################################

/*
* Function to initialize the global info needed to to print a message to the screen.
*
* Parameters:
*   uint32 f_size: the scale whith which the font will be drawn to the screen
*/
void init_print(uint32_t f_size)
{
  font_size = f_size;
  selected_font = &basic_font[0];
  charBuffer = (uint32_t*) memory_alloc(font_size*font_size*CHAR_BITS*CHAR_BITS);
}

/*
* Function to create a character image that can be drawn to the screen.
*
* Parameters:
*   const char* charMap: pointer to an array of bytes containing the display info of 
*                       a character.  
*   uint32_t size: The scale of the final character
*   uint32_t drawnCHar: A pointer to a section of memory big enough to store the 
*                      final char image.
*/
void init_char_image(const char* charMap, uint32_t size, uint32_t* drawnChar)
{
  // iterate through every pixel row in the final char image
  for(uint32_t i=0; i<CHAR_BITS*size; i++)
  {
    /*
    * Select the row in the original character info corresponding to the current
    * row in the final scaled image. (i.e the same row in the original info must be
    * duplicated multiple times to fill the final scaled image).
    */
    uint32_t line = charMap[i/size];  
    // iterate through every column in teh final char image
    for(uint32_t j=0; j<CHAR_BITS*size; j++)
    {
      /* 
      * The current pixel in the final image becomes the first bit in the current line
      * multiplied by the color (we are setting the pixel values for the FB)
      */
      drawnChar[i*CHAR_BITS*size+j] = (BIT(line, 0))*WHITE_32; // BIT(val,bit) is a bit mask
      if(j%size == 0 && j>0)
        // Every size iterations we must shift the current line to select the next bit
        line = line >> 1;
    }
  }
}

/*
* Function to draw a character to a buffer
*
* Parameter:
*   uint32_t *character image: a pointer to the buffer where the character is stored
*   uin32_t size: the scale of the character to be drawn
*   uint32_t x_offset: the horizontal offset of the character to be drawn relative to the scale.
*                     This means the character's offset is not in screen coordinates but in a 
*                     coordinate system relative to the character size and the screen dimensions.
*   uint32_t y_offset: the vertical offset of the character to be drawn relative to the scale.
*                     This means the character's offset is not in screen coordinates but in a 
*                     coordinate system relative to the character size and the screen dimensions.
*/
void drawChar(uint32_t *characterImage, uint32_t size, 
  uint32_t x_offset, uint32_t y_offset)
{
  uint32_t scaling = CHAR_BITS*size;
  // transform the given coordinates from text coordinates to screen/fb coordinates
  x_offset *= scaling;
  y_offset *= scaling;

  // iterate through every pixel in the buffer
  for(uint32_t i=0; i<scaling; i++)
  {
    for(uint32_t j=0; j<scaling; j++)
    {
      // The current framebuffer word (a pixel)
      *(volatile uint32_t *)((main_monitor.fb_ptr & ~BUS_MASK) 
        + ((i+y_offset)*main_monitor.virtual_width + (j+x_offset)) //map the char buffer value  
        *main_monitor.color_depth/CHAR_BITS) =                     //to the fb coordinates
        // The char buffer value
        characterImage[i*scaling+j];
    }
  }
}

/*
* A function to turn a number into a string
* 
* Parameters:
*   uint32_t num: the number to be converted into a string
*/
char* itos(uint32_t num)
{
  //store the original vlaue
  uint32_t original_num = num;
  int digits = 0;
  // Count the number of digits in base 10 of the given number
  do
  {
    digits++;
    num /= 10; 
  }
  while(num != 0);

  //restore the number to it's original value
  num = original_num;
  // allocate memory for the final string
  char* str = (char*)memory_alloc(sizeof(char)*(digits+1));
  str[digits--] = 0;

  //Take each digit in the number and trun it into a character
  while(digits>=0)
  {
    int digit = num % 10;
    str[digits--] = digit + '0';
    num /= 10;
  }

  return str;
}

/*
* The following functions print their parameters to the screen as a message.
* They are overloaded which means they are cpp functions and not true C functions.
*/
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
  // Select the current global cursor's position
  uint32_t x = main_cursor.x;
  uint32_t y = main_cursor.y; 

  uint32_t size = font_size;
  //loop until we find a null character
  for(uint32_t i=0; string[i] != '\0'; i++)
  { 
    //If we were to exceed the screen dimension when printing a character, go to a new line instead
    if((x) >= (main_monitor.virtual_width/(size*CHAR_BITS)))
    {
      x = 0;
      y++;
    }

    //new line character, go to next line
    if(string[i] == '\n')
    {
      x=0;
      y++;
    }
    
    //Select, initialize and draw a single character
    else
    {
      const char *currentChar = (selected_font[(uint32_t)string[i]]);
      init_char_image(currentChar, size, charBuffer);
      drawChar(charBuffer, size, x, y);
      x++;
    }

    // if we were to exceed screen dimensions go to the first line instead
    if(y >= main_monitor.virtual_height/(size*CHAR_BITS))
      y = 0;
  }

  //update the global cursor
  main_cursor.x = x;
  main_cursor.y = y;
} 