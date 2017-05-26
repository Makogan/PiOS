SECTIONS {

  .init 0x8000 : 
  {
    *(.init)
  }

  .text : 
  {
    *(.text)
  }

  .data ALIGN(0x20) : 
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
}
