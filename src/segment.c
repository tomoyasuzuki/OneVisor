#include "segment.h"
#include "cpu.h"

union segment_descriptor *gdt;

void init_segment() {
    gdt[0].control = 0;
    gdt[1].control = create_segment_descriptor(5, 0);
    gdt[2].control = create_segment_descriptor(1,0);
}

uint64_t create_segment_descriptor(uint64_t type, uint64_t dpl) {
    union segment_descriptor seg_desc;
    seg_desc.control = 0;

    seg_desc.bits.type = type;
    seg_desc.bits.dpl = dpl;
    seg_desc.bits.g = 1;
    seg_desc.bits.p = 1;
    seg_desc.bits.s = 1;

    return seg_desc.control;
}