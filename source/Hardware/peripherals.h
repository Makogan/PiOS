//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero
*
*
* File type: Header
*
* Associated files: Multiple
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#define MAIL_BASE 0xB880       // Base address for the mailbox
#define IO_BASE 0x3F000000     // Base IO address for Raspberry Pi™ model 3
#define BUS_MASK 0xC0000000

#define SYSTEM_TIMER 0x3000	   // System timer's address
#define SYS_TIME_LOW 0x4       // System timer lowest 32 bits

#endif