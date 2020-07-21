#include "bootinfo.h"
#include "window.h"
#include <stdint.h>

void vmmEntry(BootInfo *boot_info) {
    
    init_graphic(boot_info->frame_buff);
    blackout_window();
    
    char *s = "OneVisor";
    put_s(s);
}