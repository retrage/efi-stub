
.global _entry

.align 8
.text

_entry:
  movq %rdi, %rcx /* EFI_HANDLE */
  movq %rsi, %rdx /* EFI_SYSTEM_TABLE* */
  call efi_main
  ret           /* return value in rax */
