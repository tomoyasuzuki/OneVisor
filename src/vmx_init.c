#include "vmx_init.h"
#include "cpu.h"
#include "assembly.h"
#include "window.h"
#include "serial.h"

#define CPUID1 0x10
#define CPUID_VMX_BIT 0x20
#define IA32_FEATURE_CONTROL_LOCK_BIT 0x0
#define IA32_FEATURE_CONTROL_VMXON_INSIDE_SMX 0x1
#define IA32_FEATURE_CONTROL_VMXON_OUTSIDE_SMX 0x2
#define IA32_FEATURE_CONTROL 0x3a

void vmx_init() {
    union cr0_t cr0;
    union cr4_t cr4;
    uint32_t cpu_feature = cpuidf(CPUID1);
    uint64_t ia32_feature_control = read_msr(IA32_FEATURE_CONTROL);

    log_u64((uint64_t)cpu_feature);
    /* check vmx is enable or not */
    if (cpu_feature & CPUID_VMX_BIT) {
        if (ia32_feature_control & IA32_FEATURE_CONTROL_LOCK_BIT) {
            /* vmx is enable　*/
            put_s("vmx is enable");
        }
    } else {
        put_s("vmx is not enable");
        return;
    }

    cr0.control = read_cr0();
    cr4.control = read_cr4();
    
    cr4.bits.vmxe = 1;
    write_cr4(cr4.control);

    put_s("vmxon");
}