#include <stdint.h>

uint64_t read_cr3();
void write_cr3(uint64_t value);
uint64_t read_cr0();
void write_cr0(uint64_t value);
uint64_t read_cr4();
void write_cr4(uint64_t value);
uint8_t io_in8(uint16_t addr);
void io_out8(uint16_t addr, uint8_t value);
uint32_t io_in32(uint16_t addr);
void io_out32(uint16_t addr, uint32_t value);
void load_gdt(uint64_t offset, uint16_t limit);
void set_ds(uint16_t value);
void set_cs(uint16_t cs);
void load_idt(uint16_t limit, uint64_t offset);
uint32_t cpuidf(uint64_t id);
uint64_t read_msr(uint32_t addr);
void exec_vmxon(uint64_t id);
void disable_a20();
void write_msr(uint32_t addr, uint64_t value);
void exec_vmxoff();
uint8_t check_cf();
uint8_t check_zf();
uint8_t check_pf();
uint8_t check_sf();
uint16_t read_flags();

