#include "interrupt.h"
#include "segment.h"
#include "assembly.h"
#include "serial.h"

struct interrupt_descriptor idt[256];

void init_idt() {

    // send_serials("idt entry: ");
    // log_u64((uint64_t)&idt);
    // send_serials("idt last: ");
    // log_u64((uint64_t)&idt + sizeof(idt));
    // send_serials("gp location: ");
    // log_u64((uint64_t)&idt + 12 * sizeof(struct interrupt_descriptor));
    set_idt_entry(&idt[0], (uint64_t)de_handler);
    set_idt_entry(&idt[1], (uint64_t)db_handler);
    set_idt_entry(&idt[3], (uint64_t)bp_handler);
    set_idt_entry(&idt[4], (uint64_t)of_handler);
    set_idt_entry(&idt[5], (uint64_t)br_handler);
    set_idt_entry(&idt[6], (uint64_t)ud_handler);
    set_idt_entry(&idt[7], (uint64_t)nm_handler);
    set_idt_entry(&idt[8], (uint64_t)df_handler);
    set_idt_entry(&idt[10], (uint64_t)ts_handler);
    set_idt_entry(&idt[11], (uint64_t)np_handler);
    set_idt_entry(&idt[12], (uint64_t)ss_handler);
    set_idt_entry(&idt[13], (uint64_t)gp_handler);
    set_idt_entry(&idt[14], (uint64_t)pf_handler);
    set_idt_entry(&idt[16], (uint64_t)mf_handler);
    set_idt_entry(&idt[17], (uint64_t)ac_handler);
    set_idt_entry(&idt[18], (uint64_t)mc_handler);
    set_idt_entry(&idt[19], (uint64_t)xm_handler);
    set_idt_entry(&idt[20], (uint64_t)ve_handler);
    set_idt_entry(&idt[21], (uint64_t)cp_handler);

    load_idt(sizeof(idt) - 1, (uintptr_t)(&idt[0]));
}

void set_idt_entry(struct interrupt_descriptor *desc, uint64_t handler) {
    union interrupt_descriptor_attr attr;
    attr.data = 0;
    attr.bits.descriptor_type = 14; // interrupt gate
    attr.bits.dpl = 0;
    attr.bits.present_flag = 1;
    desc->attr = attr;
    desc->offset_low = handler & 0xffffu;
    desc->offset_middle = (handler >> 16) & 0xffffu;
    desc->offset_high = handler >> 32;
    desc->segment_selector = vmmCS;
}

void halt(void) {
    while (1) __asm__("hlt");
}

void log_error(struct interrupt_frame *frame, char *error_type) {
    char *rip = "rip: ";
    char *cs = "cs: ";  
    char *rflags = "rflags: ";
    char *rsp = "rsp: ";
    char *ss = "ss: ";

    send_serials(error_type);
    send_serials(rip);
    log_u64(frame->rip);
    send_serials(cs);
    log_u64(frame->cs);
    send_serials(rflags);
    log_u64(frame->rflags);
    send_serials(rsp);
    log_u64(frame->rsp);
    send_serials(ss);
    log_u64(frame->ss);

    halt();
}

__attribute__((interrupt))
void de_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#DE Exception\n");
    halt();
}

__attribute__((interrupt))
void db_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#DB Exception\n");
    halt();
}

__attribute__((interrupt))
void bp_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#BP Exception\n");
    halt();
}

__attribute__((interrupt))
void of_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#OF Exception\n");
    halt();
}

__attribute__((interrupt))
void br_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#BR Exception\n");
    halt();
}   

__attribute__((interrupt))
void ud_handler(struct interrupt_frame *frame) {
    
    log_char("ud");
    //log_error(frame, (char*)"#UD Exception\n");
    halt();

}

__attribute__((interrupt))
void nm_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#NM Exception\n");
    halt();
}

__attribute__((interrupt))
void df_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#DF Exception\n");
    halt();
}

__attribute__((interrupt))
void ts_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#TS Exception\n");
    halt();
}

__attribute__((interrupt))
void np_handler(struct interrupt_frame *frame) {
    //while(1)
    log_error(frame, (char*)"#NP Exception\n");
    halt();
}

__attribute__((interrupt))
void ss_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#SS Exception\n");
    halt();
}

__attribute__((interrupt))
void gp_handler(struct interrupt_frame *frame) {
    
    log_char("gp");
    //log_error(frame, (char*)"#GP Exception\n");
    while(1);
}

__attribute__((interrupt))
void pf_handler(struct interrupt_frame *frame) {
    
    log_char("pf");
    //log_error(frame, (char*)"#PF Exception\n");
    halt();
}

__attribute__((interrupt))
void reserved_handler(struct interrupt_frame *frame) {
    //while(1)
    log_error(frame, (char*)"#RESERVED Exception\n");
    halt();
}

__attribute__((interrupt))
void mf_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#MF Exception\n");
    halt();
}

__attribute__((interrupt))
void ac_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#AC Exception\n");
    halt();
}

__attribute__((interrupt))
void mc_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#MC Exception\n");
    halt();
}

__attribute__((interrupt))
void xm_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#XM Exception\n");
    halt();
}

__attribute__((interrupt))
void ve_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#VE Exception\n");
    halt();
}

__attribute__((interrupt))
void cp_handler(struct interrupt_frame *frame) {
    
    log_error(frame, (char*)"#CP Exception\n");
    halt();
}