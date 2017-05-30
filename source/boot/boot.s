//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero Nasir Osman
*
*
* File type: ARM Assembly
*
* This file contains assembly code needed to initialize the hardware of the
*Raspberry PI™ before execution of the main kernel loop
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.section .init
.global _start

_start:
	ldr sp, =8000 // initialize stack pointer
	b kernel_main
