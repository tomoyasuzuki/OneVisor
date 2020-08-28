#include "bootinfo.h"
#include "window.h"
#include "memory.h"
#include "assembly.h"
#include "paging.h"
#include "serial.h"
#include <stdint.h>

void vmmEntry(BootInfo *boot_info) {
    //init_paging();
    init_serial();
    init_graphic(boot_info->frame_buff);
    blackout_window();
    
    char *entry_message = "VMM starting...";
    put_s(entry_message);

    send_serials("serial port connected.");
}