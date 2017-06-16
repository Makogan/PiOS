//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: C++
*
* This file contains the main loop for the PiOS kernel and some small
* helper functions
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
* Includes and macro definitions:
*/
//========================================================================================

#include <stdint.h>
#include <mailbox.h>
#include <peripherals.h>
#include <string.h>
#include <time.h>
#include <memory_management.h>

//########################################################################################

//========================================================================================
/**
  Function Declarations:
*/
//========================================================================================

//########################################################################################

//========================================================================================
/*
* Function Definitions:
*/
//========================================================================================

extern uint32_t Kernel_End;
extern char *char_buffer;
/*
* The main control loop for the kernel. We will never exit this function.
*/
extern "C" void kernel_main(void)
{
  init_display();
  init_print(10);
  print("Nobody knows how hard this \n!@#$\%^&*()_+ is!!!\n");
  print((uint32_t)char_buffer);
  print("\n");

  print(main_monitor.fb_ptr & ~BUS_MASK);
  print("\n");
  print("yyyyy\n");

  while(1)
  {
   blink();
  }

}
//########################################################################################
