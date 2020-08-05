#pragma once
#include <stdint.h>
#include "memorymap.h"

typedef struct Chunk {
    uint64_t prev_size;
    uint64_t size;
    struct Chunk *prev;
    struct Chunk *next;
} Chunk;

void init_memory(MemoryMap *memmap);

