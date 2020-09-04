#include "interrupt.h"
#include "segment.h"
#include "assembly.h"

struct interrupt_descriptor idt[256];

void init_idt() {
    
    clear_idt();

    set_idt_entry(0, (uint16_t)de_handler);
    set_idt_entry(1, (uint16_t)db_handler);
    set_idt_entry(2, (uint16_t)nmi_handler);
    set_idt_entry(3, (uint16_t)bp_handler);
    set_idt_entry(4, (uint16_t)of_handler);
    set_idt_entry(5, (uint16_t)br_handler);
    set_idt_entry(6, (uint16_t)ud_handler);
    set_idt_entry(7, (uint16_t)nm_handler);
    set_idt_entry(8, (uint16_t)df_handler);
    set_idt_entry(10, (uint16_t)ts_handler);
    set_idt_entry(11, (uint16_t)np_handler);
    set_idt_entry(12, (uint16_t)ss_handler);
    set_idt_entry(13, (uint16_t)gp_handler);
    set_idt_entry(14, (uint16_t)pf_handler);
    set_idt_entry(16, (uint16_t)mf_handler);
    set_idt_entry(17, (uint16_t)ac_handler);
    set_idt_entry(18, (uint16_t)mc_handler);
    set_idt_entry(19, (uint16_t)xm_handler);

    load_idt(sizeof(idt) - 1, (uint64_t)(&idt[0]));
}

void clear_idt() {
    for (int i = 0; i < 256; i++) {
        idt[i].attr.data = 0;
        idt[i].offset_low = 0;
        idt[i].offset_middle = 0;
        idt[i].offset_high = 0;
        idt[i].segment_selector = 0;
    }
}

union interrupt_descriptor_attr make_inttrupt_desc_attr(uint8_t type, uint8_t flag) {
    union interrupt_descriptor_attr attr;
    attr.data = 0;
    attr.bits.descriptor_type = type; // interrupt gate
    attr.bits.dpl = 0;
    attr.bits.present_flag = flag;
    return attr;
}

void set_idt_entry(int offset, uint64_t handler) {
    idt[offset].attr = make_inttrupt_desc_attr(14, 1);
    idt[offset].offset_low = offset & 0xffffu;
    idt[offset].offset_middle = (offset >> 16) & 0xffffu;
    idt[offset].offset_high = offset >> 32;
    idt[offset].segment_selector = vmmCS;
}

void de_handler() {

}

void db_handler() {

}

void nmi_handler() {

}

void bp_handler() {

}

void of_handler() {

}

void br_handler() {

}

void ud_handler() {

}

void nm_handler() {

}

void df_handler() {

}

void mf_handler() {

}

void ts_handler() {

}

void np_handler() {

}

void ss_handler() {

}

void gp_handler() {

}

void pf_handler() {

}

void reserved_handler() {

}

void mf_handler() {

}

void ac_handler() {

}

void mc_handler() {

}

void xm_handler() {

}