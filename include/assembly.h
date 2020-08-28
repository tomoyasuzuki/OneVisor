#include <stdint.h>

uint64_t read_cr3();
void write_cr3(uint64_t value);
uint64_t read_cr0();
void write_cr0(uint64_t value);
uint8_t io_in8(uint16_t addr);
void io_out8(uint16_t addr, uint8_t value);
