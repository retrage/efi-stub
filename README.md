# efi-stub

Linux EFI Stub like standalone UEFI application sample.

As a UEFI application is Portable Executable (PE) format,
most of UEFI application binaries are generated by using toolchains
that support PE binary output.
efi-stub uses Executable and Linkable Format (ELF) toolchains
to build UEFI application binary.
It does not require any PE toolchains.

## Getting Started

### Build

```
$ make
```

### Run

Requirements:

* QEMU
* OVMF

```
$ qemu-system-x86_64 \
  -bios OVMF.fd \
  -drive format=raw,file=fat:rw:. \
  -nodefaults \
  -display none \
  -serial stdio
```

## License

efi-stub is release under the [MIT license](LICENSE).
