#pragma once
#include <stdint.h>

union interrupt_descriptor_attr {
    uint8_t data;
    struct {
        uint8_t descriptor_type : 3;
        uint8_t size : 1;
        uint8_t : 1;
        uint8_t dpl : 2;
        uint8_t present_flag : 1;
    } bits;
};

struct interrupt_descriptor {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    union interrupt_descriptor_attr attr;
    uint16_t offset_middle;
    uint32_t offset_high;
} interrupt_descriptor;

void init_idt();
void set_idt_entry(int offset, uint64_t handler);
void clear_idt();

void de_handler();
void db_handler();
void nmi_handler();
void bp_handler();
void of_handler();
void br_handler();
void ud_handler();
void nm_handler();
void df_handler();
void mf_handler();
void ts_handler();
void np_handler();
void ss_handler();
void gp_handler();
void pf_handler();
void reserved_handler();
void mf_handler();
void ac_handler();
void mc_handler();
void xm_handler();