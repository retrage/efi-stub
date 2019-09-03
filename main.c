#include <Uefi.h>
#include <GlobalTable.h>

#include "efi.h"

EFI_STATUS
EFIAPI
efi_main (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  gImageHandle = ImageHandle;
  gST = SystemTable;
  gBS = gST->BootServices;

  efi_call2(gST->ConOut->OutputString, gST->ConOut, L"Hello world!\r\n");

  return EFI_SUCCESS;
}
