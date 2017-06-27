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
#include <interrupts.h>

//########################################################################################

//========================================================================================
/*
* Function Definitions:
*/
//========================================================================================

/*
* The main control loop for the kernel. We will never exit this function.
*/
extern "C" void kernel_main(void)
{
  //Enable interrupts
  interrupt_enable_I();
  //Enable system timer irq
  enable_select_irq(32+1);
  //Set the timer irq to 10 seconds
  set_time_irq(10000000);

  print("Pi OS: This message exists to showsome behaviour ofthe PiOS\n");

  while(1)
  {
   blink();
  }

}
//########################################################################################
