#pragma once
#include <stdint.h>

union interrupt_descriptor_attr {
    uint16_t data;
    struct {
        uint16_t : 8;
        uint16_t descriptor_type : 4;
        uint16_t : 1;
        uint16_t dpl : 2;
        uint16_t present_flag : 1;
    } bits;
} __attribute__((packed));

struct interrupt_descriptor {
    uint16_t offset_low;
    uint16_t segment_selector;
    union interrupt_descriptor_attr attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct interrupt_frame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

void init_idt();
void set_idt_entry(struct interrupt_descriptor *desc, uint64_t handler);
void log_error(struct interrupt_frame *frame, char *error_type);
void de_handler(struct interrupt_frame *frame);
void db_handler(struct interrupt_frame *frame);
void bp_handler(struct interrupt_frame *frame);
void of_handler(struct interrupt_frame *frame);
void br_handler(struct interrupt_frame *frame);
void ud_handler(struct interrupt_frame *frame);
void nm_handler(struct interrupt_frame *frame);
void df_handler(struct interrupt_frame *frame);
void ts_handler(struct interrupt_frame *frame);
void np_handler(struct interrupt_frame *frame);
void ss_handler(struct interrupt_frame *frame);
void gp_handler(struct interrupt_frame *frame);
void pf_handler(struct interrupt_frame *frame);
void reserved_handler(struct interrupt_frame *frame);
void mf_handler(struct interrupt_frame *frame);
void ac_handler(struct interrupt_frame *frame);
void mc_handler(struct interrupt_frame *frame);
void mv_handler(struct interrupt_frame *frame);
void xm_handler(struct interrupt_frame *frame);
void ve_handler(struct interrupt_frame *frame);
void cp_handler(struct interrupt_frame *frame);