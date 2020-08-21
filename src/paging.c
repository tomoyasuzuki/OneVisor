#include <stdint.h>
#include "paging.h"
#include "assembly.h"
#include <stdalign.h>
#include "cpu.h"

#define PageSize4K 4096
#define PageSize2M 512 * PageSize4K
#define PageSize1G 512 * PageSize2M

alignas(PageSize4K) uint64_t pml4_table[512];
alignas(PageSize4K) uint64_t pdp_table[512];
alignas(PageSize4K) uint64_t page_directory[4][512];

void init_paging() {
    pml4_table[0] = (uint64_t)(&pdp_table[0]) | 0x003;
    for (int i_pdpt = 0; i_pdpt < (int)sizeof(page_directory); ++i_pdpt) {
        pdp_table[i_pdpt] = (uint64_t)(&page_directory[i_pdpt]) | 0x003;
        for (int i_pd = 0; i_pd < 512; ++i_pd) {
            page_directory[i_pdpt][i_pd] = i_pdpt * PageSize1G + i_pd * PageSize2M | 0x083;
        }
    }

    union cr0 current_cr0 = (union cr0)read_cr0();
    current_cr0.bits.wp = 1;
    write_cr3((uint64_t)&pml4_table[0]);
    write_cr0(current_cr0.control);
}


