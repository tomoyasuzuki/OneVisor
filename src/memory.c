#include "window.h"
#include "memorymap.h"
#include <stdint.h>
#include "assembly.h"
#include "serial.h"
#include <stdalign.h>
#include "cpu.h"
#include "memory.h"

#define PageSize4K 4096
#define PageSize2M 512 * PageSize4K
#define PageSize1G 512 * PageSize2M
#define NumOfPDPT 32

enum page_type {
    kAllocated,
    kFree
};

typedef struct _page {
    struct _page *next;
    struct _page *prev;
    enum page_type type;
}page_t; 


page_t base_p = {NULL, NULL, 1};
int pheader_size = sizeof(page_t);

alignas(PageSize4K) uint64_t pml4_table[512];
alignas(PageSize4K) uint64_t pdp_table[NumOfPDPT];
alignas(PageSize4K) uint64_t page_directory[NumOfPDPT][512];

void init_paging() {
    pml4_table[0] = (uint64_t)(&pdp_table[0]) | 0x003;
    for (int i_pdpt = 0; i_pdpt < NumOfPDPT; ++i_pdpt) {
        pdp_table[i_pdpt] = (uint64_t)(&page_directory[i_pdpt]) | 0x003;
        for (int i_pd = 0; i_pd < 512; ++i_pd) {
            page_directory[i_pdpt][i_pd] = (uint64_t)i_pdpt * PageSize1G + (uint64_t)i_pd * PageSize2M | 0x083;
        }
    }

    union cr0_t current_cr0;
    current_cr0.control = read_cr0();
    current_cr0.bits.wp = 1;
    current_cr0.bits.pe = 1;
    current_cr0.bits.pg = 1;
    //current_cr0.bits.ne = 1;
    write_cr3((uint64_t)&pml4_table[0]);
    write_cr0(current_cr0.control);
}

void log_p(page_t *p) {
    send_serials((char*)"p: ");
    log_u64((uint64_t)p);
}

void log_base_p() {
    log_p(&base_p);
}

void log_memmap(MemoryDescriptor *desc) {
    send_serials((char*)"start addr: ");
    log_u64((uint64_t)desc->physical_addr);
    send_serials((char*)"end addr: ");
    log_u64((uint64_t)desc->physical_addr + PageSize4K * desc->num_of_pages);
}

void init_memory(MemoryMap *memmap) {
    uint64_t base = (uint64_t)memmap->buff;
    page_t *p = &base_p;
    page_t *tmp_p = p;

    for (uint64_t i = base; i < base + memmap->size; i += memmap->desc_size) {
        MemoryDescriptor *mem_desc = (MemoryDescriptor*)i;
        uint64_t p_offset = PageSize4K;
        uint64_t p_base = mem_desc->physical_addr;

        p = (page_t*)p_base;
        tmp_p->next = p;

        if (mem_desc->type == kEfiConventionalMemory) {
            //log_memmap(mem_desc);

            page_t *pp = p; 
            int count = 0;
            while(count < mem_desc->num_of_pages) {
                //log_p(pp);
                pp->next = (page_t*)((uint64_t)pp + p_offset);
                pp->next->next = NULL;
                pp->next->type = kFree;
                pp->next->prev = pp;
                pp = pp->next;
                count++;
            }

            tmp_p = pp;
            // 空きページ数が多すぎるので、現時点では最初の領域だけ確保する
            // 実機では159ページある
            break;
        }
    }
}

void *alloc_page() {
    return alloc_pages(1);
}

void *alloc_pages(int num_of_pages) {
    int flag = 0;
    page_t *p = &base_p;
    void *ptr;

    while(!flag && p != NULL) {
        if (p->type == kFree) {
            int free_p_count = 1;
            int p_count = 0;
            ptr = (void*)p;
            while(p_count < num_of_pages - 1) {
                p = p->next;
                p_count++;
                if (p->type != kFree) break;
                send_serials((char*)"free: ");
                log_u64((uint64_t)p);
                free_p_count++;
            }

            if (free_p_count == num_of_pages) {
                int mark_count = 0;
                while(mark_count < num_of_pages) {
                    p->type = kAllocated;
                    send_serials((char*)"marked: ");
                    log_u64((uint64_t)p);
                    p = p->prev;
                    mark_count++;
                }
                flag = 1;
            }
        }
        p = p->next;
    }

    if (!flag) return NULL;
    return (void*)((uint64_t)ptr);
    //return (void*)((uint64_t)ptr + pheader_size);
};

void free_page(void *ptr) {
    return free_pages(ptr, 1);
}

void free_pages(void *ptr, int num) {
    page_t *p = (page_t*)((uint64_t)ptr - pheader_size);
    int free_count = 0;
    while(free_count < num) {
        p->type = kFree;
        send_serials((char*)"freed: ");
        log_u64((uint64_t)p);
        p = p->next;
        free_count++;
    }
}