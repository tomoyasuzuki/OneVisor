#include "segment.h"
#include "cpu.h"
#include "assembly.h"
#include "serial.h"
#include "memory.h"

uint64_t gdt[7];
uint32_t tss[26];

void init_segment() {
    gdt[0] = 0;
    gdt[1] = createCS(10, 0); // code segment
    gdt[2] = createDS(2,0); // data segment

    load_gdt((uint64_t)(&gdt[0]), sizeof(gdt) - 1);

    set_cs(vmmCS, vmmSS);
    set_ds(0);
    log_char("initialize segments.");
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
    desc.bits.db = 1;
    
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
    
    log_u64(desc.control);

    return desc.control; 
}

void set_tss(int index, uint64_t val) {
    tss[index] = val & 0xffffffff;
}

uint64_t alloc_stack(int num) {
    uint64_t addr = (uint64_t)alloc_pages(num);
    return addr + num * 4096; // スタックは上位アドレスから下位アドレスへ伸長するため
}

uint64_t create_sys_seg(uint64_t type, uint64_t dpl, uint64_t base, uint64_t limit) {
    union segment_descriptor desc;

    desc.control = 0;
    desc.bits.type = type;
    desc.bits.dpl = dpl;
    desc.bits.g = 1;
    desc.bits.p = 1;
    desc.bits.s = 1;
    desc.bits.db = 1; // 32bit stack segment

    desc.bits.base1 = base & 0xffffu;
    desc.bits.base2 = (base >> 16) & 0xffu;
    desc.bits.base3 = (base >> 24) & 0xffu;

    desc.bits.limit_low = limit & 0xffffu;
    desc.bits.limit_high = limit & 0xffffu;


    return desc.control;
}

void init_tss() {
    uint64_t stack_addr = alloc_stack(8);
    set_tss(1, stack_addr);

    uint64_t tss_addr = (uint64_t)&tss[0];
    gdt[ktss >> 3] = create_sys_seg(9, 0, tss_addr & 0xffffffff, sizeof(tss) - 1);

    loadtr(ktss);
}