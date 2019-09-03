/* SPDX-License-Identifier: MIT */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "pe.h"

#define ALIGN_UP(val, align) ((val + align - 1) & ~(align - 1))

const size_t ALIGN = 0x1000;
const size_t STRSIZE = 1024;

unsigned long long offset;
unsigned int address_of_entry_point;
unsigned int size_of_code;
unsigned int base_of_code;
unsigned int size_of_image;
unsigned int text_size;
unsigned int text_addr;
unsigned int rodata_size;
unsigned int rodata_addr;
unsigned int data_size;
unsigned int data_addr;
unsigned int bss_size;
unsigned int bss_addr;

static void parse_symbol_table(const char *sym_name)
{
  FILE *sym_fp = fopen(sym_name, "r");
  if (!sym_fp) {
    fprintf(stderr, "Could not open file %s\n", sym_name);
    return;
  }

  const char *HEADER = "_hdr";
  const char *ENTRY = "_entry";
  const char *TEXT = "_text";
  const char *ETEXT = "_etext";
  const char *RODATA = "_rodata";
  const char *ECTORS = "_ectors";
  const char *DATA = "_data";
  const char *EDATA = "_edata";
  const char *BSS = "__bss_start";
  const char *END = "_end";

  unsigned int text;
  unsigned int etext;
  unsigned int rodata;
  unsigned int ectors;
  unsigned int data;
  unsigned int edata;
  unsigned int bss;
  unsigned int end;

  unsigned long long addr;
  char c;
  char sym[STRSIZE];
  while (fscanf(sym_fp, "%llx %c %s", &addr, &c, sym) != EOF) {
    if (!strncmp(sym, HEADER, IMAGE_SIZEOF_SHORT_NAME)) {
      offset = addr;
    } else if (!strncmp(sym, ENTRY, IMAGE_SIZEOF_SHORT_NAME)) {
      address_of_entry_point = addr - offset;
    } else if (!strncmp(sym, TEXT, IMAGE_SIZEOF_SHORT_NAME)) {
      text = addr - offset;
    } else if (!strncmp(sym, ETEXT, IMAGE_SIZEOF_SHORT_NAME)) {
      etext = addr - offset;
    } else if (!strncmp(sym, RODATA, IMAGE_SIZEOF_SHORT_NAME)) {
      rodata = addr - offset;
    } else if (!strncmp(sym, ECTORS, IMAGE_SIZEOF_SHORT_NAME)) {
      ectors = addr - offset;
    } else if (!strncmp(sym, DATA, IMAGE_SIZEOF_SHORT_NAME)) {
      data = addr - offset;
    } else if (!strncmp(sym, EDATA, IMAGE_SIZEOF_SHORT_NAME)) {
      edata = addr - offset;
    } else if (!strncmp(sym, BSS, IMAGE_SIZEOF_SHORT_NAME)) {
      bss = addr - offset;
    } else if (!strncmp(sym, END, IMAGE_SIZEOF_SHORT_NAME)) {
      end = addr - offset;
    }
   }

  base_of_code = text;
  size_of_code = etext - text;
  size_of_image = end;

  text_size = ALIGN_UP(size_of_code, ALIGN);
  text_addr = text;
  rodata_size = ALIGN_UP(ectors - rodata, ALIGN);
  rodata_addr = rodata;
  data_size = ALIGN_UP(edata - data, ALIGN);
  data_addr = data;
  bss_size = ALIGN_UP(end - bss, ALIGN);
  bss_addr = bss;

  fclose(sym_fp);
}

int main(int argc, char *argv[])
{
  if (argc < 4) {
    fprintf(stderr, "Usage: %s SymbolTable INPUT OUTPUT\n", argv[0]);
    return 1;
  }

  parse_symbol_table(argv[1]);

  char bin_name[STRSIZE];
  strncpy(bin_name, argv[2], STRSIZE);
  int bin_fd = open(bin_name, O_RDONLY);
  if (bin_fd == -1) {
    fprintf(stderr, "Could not open file %s\n", bin_name);
    goto fail;
  }

  FILE *bin_fp = fdopen(bin_fd, "rb");
  if (!bin_fp) {
    fprintf(stderr, "Could not open file %s\n", bin_name);
    goto fail;
  }

  struct stat bin_st;
  if (fstat(bin_fd, &bin_st) == -1) {
    fprintf(stderr, "fstat failed\n");
    goto fail;
  }

  size_t bin_size = bin_st.st_size;
  void *bin_buf = malloc(bin_size);
  if (!bin_buf) {
    fprintf(stderr, "Could not allocate memory\n");
    goto fail;
  }

  if (fread(bin_buf, 1, bin_size, bin_fp) != bin_size) {
    fprintf(stderr, "Cound not read file\n");
    goto fail;
  }

  IMAGE_DOS_HEADER *doshdr = (IMAGE_DOS_HEADER *)bin_buf;
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

  opthdr->AddressOfEntryPoint = address_of_entry_point;
  opthdr->SizeOfCode = size_of_code;
  opthdr->BaseOfCode = base_of_code;
  opthdr->SizeOfImage = size_of_image;

  for (int i = 0; i < fhdr->NumberOfSections; i++) {
    IMAGE_SECTION_HEADER *sechdr \
      = ((void *)nthdr + sizeof(IMAGE_NT_HEADERS)
          + sizeof(IMAGE_SECTION_HEADER) * i);
    if (!strncmp(sechdr->Name, ".text", 8)) {
      sechdr->Misc.VirtualSize = text_size;
      sechdr->VirtualAddress = text_addr;
      sechdr->SizeOfRawData = text_size;
      sechdr->PointerToRawData = text_addr;
    } else if (!strncmp(sechdr->Name, ".rodata", 8)) {
      sechdr->Misc.VirtualSize = rodata_size;
      sechdr->VirtualAddress = rodata_addr;
      sechdr->SizeOfRawData = rodata_size;
      sechdr->PointerToRawData = rodata_addr;
    } else if (!strncmp(sechdr->Name, ".data", 8)) {
      sechdr->Misc.VirtualSize = data_size;
      sechdr->VirtualAddress = data_addr;
      sechdr->SizeOfRawData = data_size;
      sechdr->PointerToRawData = data_addr;
    } else if (!strncmp(sechdr->Name, ".bss", 8)) {
      sechdr->Misc.VirtualSize = bss_size;
      sechdr->VirtualAddress = bss_addr;
      sechdr->SizeOfRawData = bss_size;
      sechdr->PointerToRawData = bss_addr;
    }
  }

  void *wbuf = malloc(size_of_image);
  memset(wbuf, 0, size_of_image);
  memcpy(wbuf, bin_buf, size_of_image);

  FILE *wfp = fopen(argv[3], "wb");
  fwrite(wbuf, size_of_image, 1, wfp);
  fclose(wfp);

  return 0;

fail:
  if (bin_buf)
    free(bin_buf);
  if (bin_fp)
    fclose(bin_fp);
  if (bin_fd != -1)
    close(bin_fd);

  return 1;
}
