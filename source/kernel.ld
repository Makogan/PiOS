SECTIONS 
{
 .init 0x8000 : 
  {
    KEEP(*(.init))
  }

  .text : 
  {
    . = ALIGN(4);
    __text_start__ = .;              /* Just a label to get text section start addr if you need */
    *(.text .text.* .gnu.linkonce.t.*)
    . = ALIGN(4);
    __text_end__ = .;               /* Just a label to get text section end addr if you need */
  }

  .data : 
  {
    . = ALIGN(4);
    __data_start__ = .;            /* Just a label to get data section start addr if you need */
    *(.data .data.* .gnu.linkonce.d.*)
    . = ALIGN(4);
    __data_end__ = .;             /* Just a label to get data section end addr if you need */
  }

  .bss :
  {
      . = ALIGN(4);
      __bss_start__ = .;
      KEEP(*(.bss))
      . = ALIGN(4);
      __bss_end__ = .;
  }

  /* Made by LDB */
  .stack :
  {
      . = ALIGN(8);  /* Stack must always be aligned to 8 byte boundary AAPCS call standard */
      __stack_start__ = .;
      . = . + 512;    /* fiq stack size */
      __fiq_stack = .;
      . = . + 16384;   /* usr & sys stack size (common) */
      __usrsys_stack = .;
      . = . + 16384;  /* svc stack size (start-up) */
      __svc_stack = .;
      . = . + 4096;   /* irq stack size */
      __irq_stack = .;
      . = . + 512;    /* mon stack size */
      __mon_stack = .;
      . = . + 512;    /* hyp stack size */
      __hyp_stack = .;
      . = . + 512;    /* und stack size */
      __und_stack = .;
      . = ALIGN(8);
      __stack_end__ = .;      
  }
/* end of LDB contribution */
  
  .Heap :
  {
    . = ALIGN(4);
    Kernel_End = .;     /* Label to mark end of kernel code*/; 
  }
}
