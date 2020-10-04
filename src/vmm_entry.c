#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "paging.h"
#include "serial.h"
#include "segment.h"
#include "interrupt.h"
#include <stdint.h>

void vmmEntry(BootInfo *boot_info) {
    init_graphic(boot_info->frame_buff);
    char *s = "VMM Start.";
    put_s(s);
    init_serial();
    send_serials(s);
    init_segment(); 
    init_idt();
    init_paging();
    char *hoge = "hoge";
    put_s(hoge);
}