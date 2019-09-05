/* SPDX-License-Identifier: MIT */

.section .data.header

_hdr:

/* DOS Header */
dos_header:
.ascii "MZ"                             /* e_magic */
.fill 0x3a, 1, 0
.long coff_header - dos_header          /* e_lfanew */

/* COFF Header */
coff_header:
.ascii "PE\0\0"                         /* Signature */

/* File Header */
file_header:
.word 0x8664                            /* Machine */
.word 0x0004                            /* NumberOfSections */
.long 0x00000000                        /* TimeDateStamp */
.long 0x00000000                        /* PointerToSymbolTable */
.long 0x00000000                        /* NumberOfSymbols */
.word section_header - optional_header  /* SizeOfOptionalHeader */
.word 0x0002                            /* Characteristics */

/* Optional Header */
optional_header:
.word 0x020b                            /* Magic */
.byte 0x00
.byte 0x00
.long 0x00000000                        /* FILLED: SizeOfCode */
.long 0x00000000
.long 0x00000000
.long 0x00000000                        /* FILLED: AddressOfEntryPoint */
.long 0x00000000                        /* FILLED: BaseOfCode */

.quad 0x0000000000000000                /* ImageBase */
.long 0x00001000                        /* SectionAlignment */
.long 0x00001000                        /* FileAlignment */
.word 0x0000
.word 0x0000
.word 0x0000
.word 0x0000
.word 0x0000
.word 0x0000
.long 0x00000000
.long 0x00000000                        /* FILLED: SizeOfImage */
.long _ehdr - _hdr                      /* SizeOfHeaders */
.long 0x00000000                        /* FIXME: Checksum */
.word 0x000a                            /* Subsystem */
.word 0x0000
.quad 0x0000000000000000
.quad 0x0000000000000000
.quad 0x0000000000000000
.quad 0x0000000000000000
.long 0x00000000
.long 0x00000010                        /* NumberOfRvaAndSizes */
.fill 0x20,4,0x00000000                 /* DataDirectory */

/* Section Header */
section_header:
.ascii ".text\0\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x60500020 /* r-x exec */

.ascii ".rodata\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x42100040 /* r-- inited */

.ascii ".data\0\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0xc8000080 /* rw- uninited */

.ascii ".bss\0\0\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0xc8000040 /* rw- inited */

.align 0x1000 /* Adjust to FileAlignment */

_ehdr:
