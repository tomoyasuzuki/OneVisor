#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include <stdint.h>

void vmmEntry(BootInfo *boot_info) {
    
    init_graphic(boot_info->frame_buff);
    blackout_window();
    
    char *success_cr3 = "Get CR3";
    char *success_cr0 = "Get CR0";
    
    uint64_t cr3 = read_cr3();
    uint64_t cr0 = read_cr0();

    if (cr3 != 0) {
        put_s(success_cr3);
    }

    if (cr0 != 0) {
        put_s(success_cr0);
    }
}