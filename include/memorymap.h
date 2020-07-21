#pragma once
#include <stdint.h>

typedef struct MemoryMap {
    void* buff;
    uint64_t size;
    uint64_t key;
    uint64_t desc_size;
    uint32_t desc_version;
} MemoryMap;
