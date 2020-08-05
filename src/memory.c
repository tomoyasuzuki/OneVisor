#include "memory.h"
#include "window.h"

void init_memory(MemoryMap *memmap) {
    uint64_t *base = (uint64_t*)memmap->buff;

    for (uint64_t i = 0; i < (uint64_t)base + memmap->size; i += memmap->desc_size) {
        MemoryDescriptor *mem_desc = (MemoryDescriptor*)base[i];
        
        if (mem_desc->type == kEfiConventionalMemory) {
            char *s = "Find EfiConventionalMemory";
            put_s(s);
        }
    }
}

void allocate(size_t size) {

}