#include <stdint.h>

uint64_t read_cr3();
void write_cr3(uint64_t value);
uint64_t read_cr0();
void write_cr0(uint64_t value);
uint64_t read_cr4();
void write_cr4(uint64_t value);
uint8_t io_in8(uint16_t addr);
void io_out8(uint16_t addr, uint8_t value);
uint32_t io_in32(uint16_t);
void io_out32(uint16_t addr, uint32_t value);
void load_gdt(uint64_t offset, uint16_t limit);
void set_ds(uint16_t value);
void set_cs(uint16_t cs);
void load_idt(uint16_t limit, uint64_t offset);
