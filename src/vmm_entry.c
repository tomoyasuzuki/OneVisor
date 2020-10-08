#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "paging.h"
#include "serial.h"
#include "segment.h"
#include "interrupt.h"
#include "vmx_init.h"
#include <stdint.h>

void vmmEntry(BootInfo *boot_info) {
    init_serial();
    log_u64(boot_info->frame_buff->baseAddr);
    init_idt();
    init_segment();
    init_paging();
    init_graphic(boot_info->frame_buff);
    char *s = "VMM Start.";
    put_s(s);
    log_char(s);
    vmx_init();
}