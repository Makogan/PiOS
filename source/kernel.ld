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

  .ARM.attributes :
  {
    *(.ARM.attributes)
  }

  .ARM.exidx :
  {
    *(.ARM.exidx)
  }

  .bss :
    {
        KEEP(*(.bss))
   }
}
