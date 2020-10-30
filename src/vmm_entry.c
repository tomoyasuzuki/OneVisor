#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "memory.h"
#include "serial.h"
#include "segment.h"
#include "interrupt.h"
#include "vmx_init.h"
#include <stdint.h>

char *hoge;
char *huga;

void vmmEntry(BootInfo *boot_info) {
    init_serial();
    init_idt();
    init_segment();
    init_paging();
    init_graphic(boot_info->frame_buff);
    char *s = "VMM Start.";
    put_s(s);    
    log_char(s);
    init_page_frames(boot_info->memmap);
    vmx_init();
    while(1);
    return;
}