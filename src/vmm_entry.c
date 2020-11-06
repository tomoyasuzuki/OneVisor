#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "serial.h"
#include "segment.h"
#include "interrupt.h"
#include "vmx_init.h"
#include <stdint.h>
#include <stdalign.h>

alignas(16) uint8_t vmm_stack[1024];

extern void vmmEntry(BootInfo *boot_info) {
    init_serial();
    init_idt();
    init_segment();
    init_paging();
    init_page_frames(boot_info->memmap);
    init_tss();
    init_graphic(boot_info->frame_buff);
    char *s = "VMM Start.";
    put_s(s);    
    log_char(s);
    vmx_init();
    while(1);
    return;
}