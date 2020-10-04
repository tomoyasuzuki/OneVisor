#include "vmx_init.h"
#include "cpu.h"
#include "assembly.h"

void vmx_init() {
    union cr0_t cr0;
    union cr4_t cr4;

    cr0 = read_cr0();
    cr4 = read_cr4();
}