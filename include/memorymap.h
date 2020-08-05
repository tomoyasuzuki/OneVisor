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
    kEfiReservedMemoryType,
    kEfiLoaderCode,
    kEfiLoaderData,
    kEfiBootServicesCode,
    kEfiBootServicesData,
    kEfiRuntimeServicesCode,
    kEfiRuntimeServicesData,
    kEfiConventionalMemory,
    kEfiUnusableMemory,
    kEfiACPIReclaimMemory,
    kEfiACPIMemoryNVS,
    kEfiACPIMemoryMappedIO,
    kEfiMemoryMappedIOPortSpace,
    kEEfiPalCode,
    kEfiPersistentMemory,
    kEfiMaxMemoryType
};


