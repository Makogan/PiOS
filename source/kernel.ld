/*
 * Very simple linker script, combing the text and data sections
 * and putting them starting at address 0x800.
 */
SECTIONS {
  /* Put the code at 0x80000, leaving room for ARM and
   * the stack. It also conforms to the standard expecations.
   */
  .init 0x8000 : 
  {
    *(.init)
  }

  .text : 
  {
    *(.text)
  }

  /* Put the data after the code */
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
