#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "paging.h"
#include "serial.h"
#include "segment.h"
#include "interrupt.h"
#include <stdint.h>

int test() {
    __asm__("divb 0");
    return 0;
}

void vmmEntry(BootInfo *boot_info) {
    init_graphic(boot_info->frame_buff);
    init_serial();
    init_segment();
    init_idt();
    init_paging();
    test();
    test();
}

//target remote localhost:1234