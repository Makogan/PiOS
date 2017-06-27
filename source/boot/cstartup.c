//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: C
*
* This file contains all the code needed to initialize the kernel libraries. It mainly
* Initializes certain global structs essential for the correct functioning of the 
* kernel and the bss section.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
* Includes and macro definitions:
*/
//========================================================================================

#include <stdint.h>
#include <mailbox.h>
#include <string.h>
#include <time.h>
#include <memory_management.h>

//########################################################################################

//========================================================================================
/*
* External symbols declarations:
*/
//========================================================================================

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t Kernel_End;

extern "C" void kernel_main();
extern "C" void init_memory_manager();
extern "C" void init_main_cursor();

//########################################################################################

//========================================================================================
/*
* Function Definitions:
*/
//========================================================================================

/*
* The main function of this file. It initializes the BSS section to 0 as required by 
* the C standard and intilizes our libraries.
*/
extern "C" void _cstartup()  __attribute__((optimize("-O0")));
void _cstartup()
{
    // __bss_start__ and __bss__ end are defined by the linker
    volatile uint32_t* current_address = &__bss_start__;
    uint32_t* end = &__bss_end__;

    // Every address in teh BSS section must be initialized to 0
    while(current_address < end)
    {
        *current_address = 0;
        current_address++;
    }

    // Initialize our libraries
    init_memory_manager();
    init_display();
    init_print(10);

    // Go to the main loop
    kernel_main();

    // We should never return from main, if we do stop here to prevent further errors
    while(1){}
}

//########################################################################################