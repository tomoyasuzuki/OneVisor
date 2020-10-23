#pragma once
#include <stdint.h>
#include <stdio.h>
#include "memorymap.h"

void init_memory(MemoryMap *memmap);
void init_paging();
void log_base_p();
void log_memmap(MemoryDescriptor *desc);
void *alloc_page();
void *alloc_pages(int num_of_pages);
void free_page(void *ptr);
void free_pages(void *ptr, int num);