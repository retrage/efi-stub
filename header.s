.section .data.boot

/* DOS Header */
dos_header:
.ascii "MZ"                     /* e_magic */
.fill 0x3a, 1, 0
.long coff_header - dos_header  /* e_lfanew */

.align 8

/* COFF Header */
coff_header:
.ascii "PE\0\0"                 /* Signature */

/* File Header */
file_header:
.word 0x8664                    /* Machine */
.word 0x0000                    /* FIXME: NumberOfSections */
.long 0x00000000                /* TimeDateStamp */
.long 0x00000000                /* PointerToSymbolTable */
.long 0x00000000                /* NumberOfSymbols */
.word 0x00f0                    /* SizeOfOptionalHeader */
.word 0x0000                    /* Characteristcs */

/* Optional Header */
optional_header:
.word 0x020b                    /* Magic */
.byte 0x00
.byte 0x00
.long 0x00000000                /* SizeOfCode */
.long 0x00000000
.long 0x00000000
.long 0x00000000                /* AddressOfEntryPoint */
.long 0x00000000                /* BaseOfCode */

.quad 0x0000000000000000        /* ImageBase */
.long 0x00000000                /* FIXME: SectionAlignment */
.long 0x00000000                /* FileAlignment */
.word 0x0000
.word 0x0000
.word 0x0000
.word 0x0000
.word 0x0000
.word 0x0000
.long 0x00000000
.long 0x00000000                /* FIXME: SizeOfImage */
.long 0x00000000                /* FIXME: SizeOfHeaders */
.long 0x00000000                /* FIXME: Checksum */
.word 0x0000                    /* FIXME: Subsystem */
.word 0x0000
.quad 0x0000000000000000
.quad 0x0000000000000000
.quad 0x0000000000000000
.quad 0x0000000000000000
.long 0x00000000
.long 0x00000010                /* NumberOfRvaAndSizes */
.fill 0x20,4,0x00000000         /* DataDirectory */

/* Section Header */
.ascii ".text\0\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x00000000

.ascii ".rodata\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x00000000

.ascii ".ctors\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x00000000

.ascii ".data\0\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x00000000

.ascii ".bss\0\0\0\0"
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.long 0x00000000
.word 0x0000
.word 0x0000
.long 0x00000000
