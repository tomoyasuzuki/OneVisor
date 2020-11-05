#include "segment.h"
#include "cpu.h"
#include "assembly.h"
#include "serial.h"

uint64_t gdt[3];

void init_segment() {
    gdt[0] = 0;
    gdt[1] = createCS(10, 0); // code segment
    gdt[2] = createDS(2,0); // data segment

    load_gdt((uint64_t)(&gdt[0]), sizeof(gdt) - 1);

    set_ds(0);
    set_cs(vmmCS);
    send_serials("idt entry: ");
}

uint64_t createDS(uint64_t type, uint64_t dpl) {
    union segment_descriptor desc;
    uint32_t base = 0;
    uint32_t limit = 0xfffff;

    desc.control = 0;
    desc.bits.type = type;
    desc.bits.dpl = dpl;
    desc.bits.g = 1;
    desc.bits.p = 1;
    
    desc.bits.base1 = base & 0xffffu;
    desc.bits.base2 = (base >> 16) & 0xffu;
    desc.bits.base3 = (base >> 24) & 0xffu;

    desc.bits.limit_low = limit & 0xffffu;
    desc.bits.limit_high = limit & 0xffffu;
    
    return desc.control;
}

uint64_t createCS(uint64_t type, uint64_t dpl) {
    union segment_descriptor desc;
    uint64_t base = 0;
    uint64_t limit = 0xfffff;

    desc.control = 0;
    desc.bits.type = type;
    desc.bits.dpl = dpl;
    desc.bits.g = 1;
    desc.bits.p = 1;
    desc.bits.s = 1;
    desc.bits.l = 1;

    desc.bits.base1 = base & 0xffffu;
    desc.bits.base2 = (base >> 16) & 0xffu;
    desc.bits.base3 = (base >> 24) & 0xffu;

    desc.bits.limit_low = limit & 0xffffu;
    desc.bits.limit_high = limit & 0xffffu;
    
    return desc.control;
}