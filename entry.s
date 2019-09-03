/* SPDX-License-Identifier: MIT */

.global _entry

.align 8
.text

_entry:
  movq %rcx, %rdi /* EFI_HANDLE */
  movq %rdx, %rsi /* EFI_SYSTEM_TABLE* */
  call efi_main
  ret           /* return value in rax */
