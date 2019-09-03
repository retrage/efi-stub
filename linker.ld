/* SPDX-License-Identifier: ISC */

ENTRY(efi_main)
SECTIONS
{
  . = 0x100000;

  /* Header */
  .header :
  {
    KEEP (*(.data.header))
  }

  /* Code */
  . = ALIGN(0x1000);
  _text = .;
  .text :
  {
    *(.text)
    *(.text.*)
  }
  _etext = .;

  /* Read-only data */
  . = ALIGN(0x1000);
  _rodata = .;
  .rodata :
  {
    *(.rodata)
    *(.rodata.*)
  }
  _erodata = .;

  /* Constructor tables (read-only) */
  _ctors = .;
  .preinit_array : {
    . = ALIGN(0x8);
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  }

  .init_array : {
    . = ALIGN(0x8);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)));
    KEEP (*(.init_array .ctors))
    PROVIDE_HIDDEN(__init_array_end = .);
  }
  _ectors = .;

  /* Read-write data (initialized) */
  . = ALIGN(0x1000);
  _data = .;
  .data :
  {
    *(.dada)
    *(.data.*)
  }
  _edata = .;

  /* Read-write data (uninitialized) */
  . = ALIGN(0x1000);
  __bss_start = .;
  .bss :
  {
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(0x1000);
  }
  _end = .;
}
