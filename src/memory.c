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

typedef struct page_frame {
    uint64_t id;
    uint64_t addr;
    enum page_type type;
} page_frame;

page_frame base_frame[1000];

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
    current_cr0.bits.pe = 0;
    current_cr0.bits.pg = 1;
    write_cr3((uint64_t)&pml4_table[0]);
    write_cr0(current_cr0.control);
    log_char("initialize page table.");
}

void log_p(page_frame *p) {
    send_serials((char*)"p: ");
    log_u64((uint64_t)p);
}

void init_page_frames(MemoryMap *memmap) {
    log_char("start initializing page frames ...");
    uint64_t base = (uint64_t)memmap->buff;
    int i_page = 0;

    for (uint64_t i = base; i < base + memmap->size; i += memmap->desc_size) {
        MemoryDescriptor *memdesc = (MemoryDescriptor*)i;

        if (memdesc->type != kEfiConventionalMemory) continue;

        uint64_t p_offset = PageSize4K;
        uint64_t area_base = memdesc->physical_addr;
        uint64_t area_last = area_base + PageSize4K * memdesc->num_of_pages;

        for (uint64_t i = area_base; i < area_last; i += p_offset) {
            page_frame p = {i, i, kFree}; // 現時点ではidをアドレスにしてしまっている
            base_frame[i_page] = p;
            i_page++;
        }

        log_char("finished.");

        break; // 空きページ数が多すぎるので、現時点では最初の領域だけ確保する
    }
}

void *alloc_page() {
    return alloc_pages(1);
}

void *alloc_pages(int num) {
    for (int i = 0; i < 1000; i++) {
        if (base_frame[i].type == kFree) {
            int free_page_count = 1;
            int j = i + 1;

            // 空きページがnum個連続している領域を探す
            while(free_page_count != num) {
                if (base_frame[j].type != kFree) break;
                free_page_count++;
                j++;
            }

            // 連続した領域を確保できなかった場合は次のループへ
            if (free_page_count != num) continue;

            for (int i_free = i; i_free < i + num; i_free++) {
                base_frame[i_free].type = kAllocated;
                send_serials("Alloc: ");
                log_u64(base_frame[i_free].addr);
            }

            send_serials("return: ");
            log_u64(base_frame[i].addr);


            return (void*)base_frame[i].addr;
        }
    }

    return NULL;
}

void free_pages(void *addr, int num) {
    int flag = 0;
    int count = 0;

    for (int i = 0; i < 1000; i++) {
        if ((base_frame[i].addr != (uint64_t)addr) && !flag) continue;
        if (!flag) flag = 1;
        base_frame[i].type = kFree;
        count++;
        send_serials("Free: ");
        log_u64(base_frame[i].addr);
        if (count == num) break;
    }
}

void free_page(void *addr) {
    return free_pages(addr, 1);
}