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

/*
* The main control loop for the kernel. We will never exit this function.
*/
extern "C" void kernel_main(void)
{
  init_display();
  print((char*)"Nobody knows how hard this \n!@#$\%^&*()_+ is!!!");

  char test[32];
  //uint32_t fps =0;
  //uint32_t old_time_cycle = get_time_cycle();

  //*(volatile uint32_t*)(0x30000000) = 7070;
  //tempitos(Kernel_End, test);
  //print(test);
  //wait(10000000);
  tempitos(*(uint32_t*)(0x30000000-4), test);
  print(test);

 /* while(1)
  {
    fps = (get_time_cycle() - old_time_cycle);
    old_time_cycle = get_time_cycle();
    tempitos(fps, test);
    print(test);
  }*/

  while(1)
  {
   blink();
  }

}
//########################################################################################
