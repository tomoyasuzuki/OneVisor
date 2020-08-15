#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "paging.h"
#include <stdint.h>

void vmmEntry(BootInfo *boot_info) {
    
    init_graphic(boot_info->frame_buff);
    blackout_window();
    
    char *entry_message = "OneVisor";
    put_s(entry_message);

    init_paging();
}