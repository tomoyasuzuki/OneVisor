#pragma once
#include <stdint.h>
#include <stdio.h>
#include "memorymap.h"

void init_page_frames(MemoryMap *memmap);
void init_paging();
void log_memmap(MemoryDescriptor *desc);
void *alloc_page();
void *alloc_pages(int num);
void free_page(void *addr);
void free_pages(void *addr, int num);