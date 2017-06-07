//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero Nasir Osman
*
*
* File type: C
*
* This files contains the main loop for the PiOS kernel and some small
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


//########################################################################################

//========================================================================================
/**
  Function Declarations:
*/
//========================================================================================

void wait(uint32_t time);

//########################################################################################

//========================================================================================
/*
* Function Definitions:
*/
//========================================================================================

/*
* The main control loop for the kernel. We will never exit this function.
*/
void kernel_main(void)
{
  volatile int tst = 0;

  /*makeuart_init();
	dbg_puts("Waiting for JTAG\r\n");
	enable_jtag();

  bcm283x_mu_serial_putc('.');*/

  /*wait(0xF0000);

  set_LED(ON);

  wait(0xF0000);

  set_LED(OFF);*/

  init_display();

  while(1)
  {
   /* wait(0xF0000);

    set_LED(ON);

    wait(0xF0000);

    set_LED(OFF);*/
  }

}

#pragma GCC optimize ("O0")
/*
* A function to wait.
*
* Parameters:
*   uint32_t time: the amount of time to wait.
*/
void wait(uint32_t time)
{
  while (time > 0)
    time--;
}
#pragma GCC optimize ("02")
//########################################################################################
