#pragma once
#include <stdint.h>
#include "memorymap.h"

typedef struct Chunk {
    uint64_t prev_size;
    uint64_t size;
    Chunk *prev;
    Chunk *next;
} Chunk;

void init_memory(MemoryMap *memmap);

