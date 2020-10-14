#pragma once
#include <stdint.h>
#include <stdio.h>
#include "memorymap.h"

void init_memory(MemoryMap *memmap);
void init_paging();
void log_base_p();
void allocate(size_t size);
void log_memmap(MemoryDescriptor *desc);