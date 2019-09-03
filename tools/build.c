/* SPDX-License-Identifier: MIT */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "pe.h"

int main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Usage: %s INPUT OUTPUT\n", argv[0]);
    goto fail;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Could not open file %s\n", argv[1]);
    goto fail;
  }

  FILE *fp = fdopen(fd, "rb");
  if (!fp) {
    fprintf(stderr, "Could not open file %s\n", argv[1]);
    goto fail;
  }

  struct stat st;
  if (fstat(fd, &st) == -1) {
    fprintf(stderr, "fstat failed\n");
    goto fail;
  }

  size_t sz = st.st_size;
  void *buf = malloc(sz);
  if (!buf) {
    fprintf(stderr, "Could not allocate memory\n");
    goto fail;
  }

  if (fread(buf, 1, sz, fp) != sz) {
    fprintf(stderr, "Cound not read file\n");
    goto fail;
  }

  const size_t peoffset = 0;
  IMAGE_DOS_HEADER *doshdr = (IMAGE_DOS_HEADER *)(buf + peoffset);
  if (doshdr->e_magic != MAGIC_MZ) {
    fprintf(stderr, "DOS header magic not found\n");
    goto fail;
  }

  IMAGE_NT_HEADERS *nthdr \
    = (IMAGE_NT_HEADERS *)((void *)doshdr + doshdr->e_lfanew);
  if (nthdr->Signature != MAGIC_PE) {
    fprintf(stderr, "PE header signature not found\n");
    goto fail;
  }

  IMAGE_FILE_HEADER *fhdr = &nthdr->FileHeader;
  IMAGE_OPTIONAL_HEADER *opthdr = &nthdr->OptionalHeader;

  const unsigned int entry_addr = 0x1010;
  opthdr->AddressOfEntryPoint = entry_addr;
  opthdr->SizeOfCode = 0x1c;
  opthdr->BaseOfCode = 0x1000;
  opthdr->SizeOfImage = 0x5000;

  for (int i = 0; i < fhdr->NumberOfSections; i++) {
    IMAGE_SECTION_HEADER *sechdr \
      = ((void *)nthdr + sizeof(IMAGE_NT_HEADERS)
          + sizeof(IMAGE_SECTION_HEADER) * i);
    if (!strncmp(sechdr->Name, ".text", 8)) {
      sechdr->Misc.VirtualSize = 0x1000;
      sechdr->VirtualAddress = 0x1000;
      sechdr->SizeOfRawData = 0x1000;
      sechdr->PointerToRawData = 0x1000;
      sechdr->Characteristics = 0x60500020; /* r-x exec */
    } else if (!strncmp(sechdr->Name, ".rodata", 8)) {
      sechdr->Misc.VirtualSize = 0x1000;
      sechdr->VirtualAddress = 0x2000;
      sechdr->SizeOfRawData = 0x1000;
      sechdr->PointerToRawData = 0x2000;
      sechdr->Characteristics = 0x42100040; /* r-- inited */
    } else if (!strncmp(sechdr->Name, ".data", 8)) {
      sechdr->Misc.VirtualSize = 0x1000;
      sechdr->VirtualAddress = 0x3000;
      sechdr->SizeOfRawData = 0x1000;
      sechdr->PointerToRawData = 0x3000;
      sechdr->Characteristics = 0xc8000080; /* rw- uninited */
    } else if (!strncmp(sechdr->Name, ".bss", 8)) {
      sechdr->Misc.VirtualSize = 0x1000;
      sechdr->VirtualAddress = 0x4000;
      sechdr->SizeOfRawData = 0x1000;
      sechdr->PointerToRawData = 0x4000;
      sechdr->Characteristics = 0xc8000080; /* rw- uninited */
    }
  }

  void *wbuf = malloc(peoffset + 0x5000);
  memset(wbuf, 0, peoffset + 0x5000);
  memcpy(wbuf, buf, peoffset + 0x5000);

  FILE *wfp = fopen(argv[2], "wb");
  fwrite(wbuf, peoffset + 0x5000, 1, wfp);
  fclose(wfp);

  return 0;

fail:
  if (buf)
    free(buf);
  if (fp)
    fclose(fp);
  if (fd != -1)
    close(fd);

  return 1;
}
