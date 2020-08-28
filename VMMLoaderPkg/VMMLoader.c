#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/UefiBootServicesTableLib.h>
#include  <Library/PrintLib.h>
#include  <Library/MemoryAllocationLib.h>
#include  <Library/BaseMemoryLib.h>
#include  <Protocol/LoadedImage.h>
#include  <Protocol/SimpleFileSystem.h>
#include  <Guid/FileInfo.h>
#include "frameBuffer.h"
#include "memorymap.h"
#include "bootinfo.h"
#include "elf.h"
#include "assm.h"

#define DEFAULT_START_ADDR (uint64_t *)0x100000

EFI_STATUS read_file(UINTN file_size, VOID **tmp_buff, EFI_FILE_PROTOCOL* elf_file) {
  EFI_STATUS status;
  
  status = gBS->AllocatePool(EfiLoaderData, file_size, tmp_buff);
  if (EFI_ERROR(status)) {
    return status;
  }

  return elf_file->Read(elf_file, &file_size, *tmp_buff);
}

VOID copy_mem(VOID* before_addr, VOID* after_addr, UINTN size) {
  EFI_STATUS status;
  UINTN num_pages = size / (1024 * 4);

  status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, num_pages, after_addr);
  if (EFI_ERROR(status)) {
    return;
  }

  return gBS->CopyMem(after_addr, before_addr, size);
}

EFI_STATUS get_memmap(MemoryMap *memmap) {
  return gBS->GetMemoryMap(
    (UINTN*)&memmap->size,
    memmap->buff,
    (UINTN*)&memmap->key,
    (UINTN*)&memmap->desc_size, 
    &memmap->desc_version);
}

EFI_STATUS open_rootdir(EFI_FILE_PROTOCOL **rootdir, EFI_HANDLE image_handle) {
  EFI_STATUS status;

  EFI_LOADED_IMAGE_PROTOCOL* image;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* filesys;

  status = gBS->OpenProtocol(
    image_handle,
    &gEfiLoadedImageProtocolGuid,
    (VOID**)&image,
    image_handle,
    NULL,
    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if (EFI_ERROR(status)) {
    return status;
  }

  status = gBS->OpenProtocol(
    image->DeviceHandle,
    &gEfiSimpleFileSystemProtocolGuid,
    (VOID**)&filesys,
    image_handle,
    NULL,
    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if (EFI_ERROR(status)) {
    return status;
  }

  return filesys->OpenVolume(filesys, rootdir);
}

EFI_STATUS open_file(EFI_FILE_PROTOCOL *rootdir, EFI_FILE_PROTOCOL **file) {
  return rootdir->Open(rootdir,file,L"vmm_entry.elf",EFI_FILE_MODE_READ, 0);
}

EFI_STATUS get_file_info(EFI_FILE_PROTOCOL *file, VOID* file_info_buff, UINTN buff_size) {
  EFI_STATUS status = EFI_SUCCESS;

  do {
    status = file->GetInfo(
      file,
      &gEfiFileInfoGuid,
      &buff_size,
      file_info_buff);
    
    buff_size += 1024;
  } while(EFI_ERROR(status));

  return status;
}

CHAR16* ConvertPixelFormatToString(EFI_GRAPHICS_PIXEL_FORMAT format) {
  switch (format)
  {
  case PixelRedGreenBlueReserved8BitPerColor:
    return L"PixelRedGreenBlueReserved8BitPerColor";
  case PixelBlueGreenRedReserved8BitPerColor:
    return L"PixelBlueGreenRedReserved8BitPerColor";
  case PixelBitMask:
    return L"PixelBitMask";
  case PixelBltOnly:
    return L"PixelBltOnly";
  case PixelFormatMax:
    return L"PixelFormatMax";
  default:
    return L"unknown format";
  }
}

EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS status;

  Print(L"Hello VMM Loader.\n");

  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
  status = gBS->LocateProtocol(
    &gEfiGraphicsOutputProtocolGuid,
    NULL,
    (VOID**)&gop
  );

  Print(L"width: %u\nheight: %u\npixel format: %s\npixels per scanline: %u\n",
    gop->Mode->Info->HorizontalResolution,
    gop->Mode->Info->VerticalResolution,
    ConvertPixelFormatToString(gop->Mode->Info->PixelFormat),
    gop->Mode->Info->PixelsPerScanLine);

  FrameBuffer frameBufferConfig = {
    (uint64_t*)gop->Mode->FrameBufferBase,
    (uint64_t)gop->Mode->FrameBufferSize,
    gop->Mode->Info->HorizontalResolution,
    gop->Mode->Info->VerticalResolution,
    gop->Mode->Info->PixelsPerScanLine,
    1
  };

    switch (gop->Mode->Info->PixelFormat)
  {
  case PixelRedGreenBlueReserved8BitPerColor:
    frameBufferConfig.pixelFormat = kPixelRGBResv8BitPerColor;
    break;
  case PixelBlueGreenRedReserved8BitPerColor:
    frameBufferConfig.pixelFormat = kPixelBGRResv8BitPerColor;
    break;
  default:
    break;
  }
  EFI_FILE_PROTOCOL* rootdir;
  status = open_rootdir(&rootdir, ImageHandle);
  Print(L"OpenRootDir: %r\n", status);

  EFI_FILE_PROTOCOL *elf_file = NULL;
  status = open_file(rootdir, &elf_file);

  Print(L"OpenFile: %r\n", status);

  UINTN buff_size = 256;
  VOID* file_info_buff[buff_size];

  status = get_file_info(elf_file, file_info_buff, buff_size);
  Print(L"GetFileInfo: %r\n", status);

  EFI_FILE_INFO *file_info = (EFI_FILE_INFO*)file_info_buff;
  UINTN file_size = file_info->FileSize;

  VOID *tmp_buff = NULL;
  read_file(file_size, &tmp_buff, elf_file);

  Print(L"ReadFile: %r\n", status);

  Elf64_Ehdr *elfh = (Elf64_Ehdr*)DEFAULT_START_ADDR;
  copy_mem(tmp_buff, (VOID*)elfh, file_size);

  Print(L"e_type: %u\n", elfh->e_type);
  Print(L"e_machine: %u\n", elfh->e_machine);
  Print(L"e_version: %u\n", elfh->e_version);
  Print(L"e_entry: 0x%x\n", elfh->e_entry);

  MemoryMap memmap = {NULL, 0, 0, 0, 0};
  CHAR8 memmap_buf[4096 * 4];

  memmap.size = sizeof(memmap_buf);
  memmap.buff = (EFI_MEMORY_DESCRIPTOR*)&memmap_buf;

  status = get_memmap(&memmap);

  Print(L"GetMemoryMap: %r\n", status);

  status = gBS->ExitBootServices(ImageHandle, memmap.key);

  if(EFI_ERROR(status)) {
    status = get_memmap(&memmap);

    while(EFI_ERROR(status)) {
      memmap.size += 512;
      status = get_memmap(&memmap);
    }
    
    status = gBS->ExitBootServices(ImageHandle, memmap.key);

    if (EFI_ERROR(status)) {
      Print(L"Failed to ExitBootServices: %r\n", status);
    }
  }

  BootInfo bootinfo = {&frameBufferConfig, &memmap};

  uint64_t *entry_addr = (uint64_t*)elfh->e_entry;
  jumpToVmm(&bootinfo, entry_addr);
  while(1);
  
  return EFI_SUCCESS;
}
