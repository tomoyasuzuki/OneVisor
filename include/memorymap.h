#pragma once
#include <stdint.h>

typedef struct MemoryMap {
    void* buff;
    uint64_t size;
    uint64_t key;
    uint64_t desc_size;
    uint32_t desc_version;
} MemoryMap;

typedef struct MemoryDescriptor {
    uint32_t type;
    uint64_t physical_addr;
    uint64_t virtual_addr;
    uint64_t num_of_pages;
    uint64_t attribute;
} MemoryDescriptor;

enum MemoryType {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiACPIMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EEfiPalCode,
    EfiPersistentMemory,
    EfiMaxMemoryType
} 


