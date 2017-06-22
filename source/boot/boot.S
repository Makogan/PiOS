//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
* Authors: Camilo Talero, LDB
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
        mov     sp, #0x8000 // initialize stack pointer
        bl  RPi_CheckAndExitHypModeToSvcMode;   ;@ First check for HYP_MODE and if in it drop out to SRV_MODE

;@"================================================================"
;@ Now setup stack pointers for the different CPU operation modes.
;@"================================================================"
        msr CPSR_c, #0xD1                  ;@ Switch to FIQ_MODE
        mov sp, #0x7000               ;@ Set the stack pointer for that mode
        msr CPSR_c, #0xD2                  ;@ Switch to IRQ_MODE
        mov sp, #0x6000               ;@ Set the stack pointer for that mode
        msr CPSR_c, #0xD3                  ;@ Switch back to SRV_MODE
        mov sp, #0x5000               ;@ Set the stack pointer for that mode

@"================================================================"
;@ PI NSACR regsister setup for access to floating point unit
;@ Cortex A-7 => Section 4.3.34. Non-Secure Access Control Register
;@ Cortex A-53 => Section 4.5.32. Non-Secure Access Control Register
;@"================================================================"
   mrc p15, 0, r0, c1, c1, 2            ;@ Read NSACR into R0
   cmp r0, #0x00000C00                  ;@ Access turned on or in AARCH32 mode and can not touch register or EL3 fault
   beq .free_to_enable_fpu
   orr r0, r0, #0x3<<10               ;@ Set access to both secure and non secure modes
   mcr p15, 0, r0, c1, c1, 2            ;@ Write NSACR
;@"================================================================"
;@ Bring fpu online
;@"================================================================"
.free_to_enable_fpu:
   mrc p15, 0, r0, c1, c0, #2            ;@ R0 = Access Control Register
   orr r0, #(0x300000 + 0xC00000)         ;@ Enable Single & Double Precision
   mcr p15,0,r0,c1,c0, #2               ;@ Access Control Register = R0
   mov r0, #0x40000000                  ;@ R0 = Enable VFP
   vmsr fpexc, r0                     ;@ FPEXC = R0
 ;@"================================================================"
;@ Enable L1 cache
;@"================================================================"
    mrc p15,0,r0,c1,c0,0               ;@ R0 = System Control Register

/*==========================================================================}
;{           RASPBERRY PI LEVEL L1 CACHE CONSTANTS DEFINED                }
;{=========================================================================*/
#define SCTLR_ENABLE_DATA_CACHE         0x4
#define SCTLR_ENABLE_BRANCH_PREDICTION   0x800
#define SCTLR_ENABLE_INSTRUCTION_CACHE  0x1000

    // Enable caches and branch prediction
    orr r0, #SCTLR_ENABLE_BRANCH_PREDICTION
    orr r0, #SCTLR_ENABLE_DATA_CACHE
    orr r0, #SCTLR_ENABLE_INSTRUCTION_CACHE

    mcr p15,0,r0,c1,c0,0               ;@ System Control Register = R0


 /* END OF CHANGES ADDED BY LdB */
 ;@"================================================================"
;@  We are getting close to handing over to C so we need to copy the 
;@  ISR table to position 0x0000 so interrupts can be used if wanted 
;@"================================================================"
_reset_:
        ldr     r0, =_v_table
        mov     r1, #0x0000
        ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
        stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
        ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
        stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
 ;@"================================================================"
;@ Now Clear the .BSS segment as the C compiler expects us to do
;@"================================================================"
   ldr   r0, =__bss_start__
   ldr   r1, =__bss_end__   
   mov   r2, #0
.clear_bss:
    cmp   r0, r1
    bge   .clear_bss_exit
    str   r2, [r0]
   add   r0, r0, #4
    b .clear_bss
.clear_bss_exit:  

    b       _cstartup


 _v_table:
        ldr     pc, _reset_h
        ldr     pc, _undefined_instruction_vector_h
        ldr     pc, _software_interrupt_vector_h
        ldr     pc, _prefetch_abort_vector_h
        ldr     pc, _data_abort_vector_h
        ldr     pc, _unused_handler_h
        ldr     pc, _interrupt_vector_h
        ldr     pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   undefined_instruction_vector
_software_interrupt_vector_h:       .word   software_interrupt_vector
_prefetch_abort_vector_h:           .word   prefetch_abort_vector
_data_abort_vector_h:               .word   _reset_
_unused_handler_h:                  .word   _reset_
_interrupt_vector_h:                .word   interrupt_vector
_fast_interrupt_vector_h:           .word   fast_interrupt_vector



 /* CODED ADDED BY LdB */
/* "PROVIDE C FUNCTION: bool RPi_CheckAndExitHypModeToSvcMode (void);" */
.section .text.RPi_CheckAndExitHypModeToSvcMode, "ax", %progbits
.balign   4
.type RPi_CheckAndExitHypModeToSvcMode, %function
.syntax unified
.arm
;@"================================================================"
;@ RPi_CheckAndExitHypModeToSvcMode -- Composite Pi1, Pi2 & Pi3 code
;@ Return: Drops out from HYP_MODE to SRV_MODE and FIQ/IRQ disabled
;@         If not in HYP_MODE will exit unchanged
;@"================================================================"
RPi_CheckAndExitHypModeToSvcMode:
   mrs r0, cpsr                     ;@ Fetch the cpsr register             
   and r1, r0, #0x1F                  ;@ Mask off the arm mode bits in register                            
    cmp r1, #0x1A                     ;@ check we are in HYP_MODE AKA register reads 1A                     
   beq .WeHaveHyperMode
   mov r0, #0;                        ;@ return false
   bx lr                           ;@ Return we are not in hypermode
.WeHaveHyperMode:
   bic r0, r0, #0x1F                  ;@ Clear the mode bits                   
   orr r0, r0, #0xD3                  ;@ We want SRV_MODE with IRQ/FIQ disabled         
   mov r1, #0                        ;@ Make sure CNTVOFF to 0 before exit HYP mode
   mcrr p15, #4, r1, r1, cr14            ;@ We do not want our clocks going fwd or backwards
   orr r0, r0, #0x100                  ;@ Set data abort mask    
   msr spsr_cxsf, r0                  ;@ Load our request into return status register
   mov r0, #1;                        ;@ return true
   bx lr
   /* I borrowed this trick from Ultibo because ARM6 code running on an ARM7/8 needs this opcode  */
   /* The ARM6 compiler does not know these instructions so it is a way to get needed opcode here */
    /* So our ARM6 code can drop an arm7 or arm8 out of HYP mode and run on an ARM7/8.             */
    /* Native ARM7/8 compilers already understand the OPCODE but do not mind it this way either      */        
   .long 0xE12EF30E                  ;@ "msr ELR_hyp, lr"
   .long 0xE160006E                  ;@ "eret"   
;@ No ltorg data required for this function 
.size   RPi_CheckAndExitHypModeToSvcMode, .-RPi_CheckAndExitHypModeToSvcMode
