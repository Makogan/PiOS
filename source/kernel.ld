SECTIONS 
{
  .vector_table 0x0 :
  {
    KEEP(*(.vector_table))
  } 

 .init 0x8000 : 
  {
    KEEP(*(.init))
  }

  .text : 
  {
    KEEP(*(.text))
  }

  .data ALIGN(0x4) : 
  {
    KEEP(*(.data))
  }

  .comment :
  {
    *(.comment)
  }

  .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        KEEP(*(.bss))
        . = ALIGN(4);
        __bss_end__ = .;
   }
}
