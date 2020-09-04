#include "segment.h"
#include "cpu.h"
#include "assembly.h"
#include "serial.h"

uint64_t gdt[3];

void init_segment() {
    gdt[0] = 0;
    gdt[1] = create_segment_descriptor(10, 0); // code segment
    gdt[2] = create_segment_descriptor(2,0); // data segment

    load_gdt((uint64_t)(&gdt[0]), sizeof(gdt) - 1);

    set_ds(0);
    set_cs(vmmCS);
}

uint64_t create_segment_descriptor(uint64_t type, uint64_t dpl) {
    union segment_descriptor seg_desc;
    seg_desc.control = 0;

    seg_desc.bits.type = type;
    seg_desc.bits.dpl = dpl;
    seg_desc.bits.g = 1;
    seg_desc.bits.p = 1;
    seg_desc.bits.s = 1;
    seg_desc.bits.l = 1;
    seg_desc.bits.db = 0;

    return seg_desc.control;
}