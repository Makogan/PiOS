SECTIONS 
{
  .vector_table 0x0 :
  {
    *(.vector_table)
  } 

 .init 0x8000 : 
  {
    KEEP(*(.init))
  }

  .text : 
  {
    *(.text .text.* .gnu.linkonce.t.*)
  }

  .data ALIGN(0x4) : 
  {
    *(.data)
  }

  .comment :
  {
    *(.comment)
  }

  .ARM.attributes :
  {
    *(.ARM.attributes)
  }

  .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
   }
}
