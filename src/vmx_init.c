#include "vmx_init.h"
#include "cpu.h"
#include "assembly.h"
#include "window.h"
#include "serial.h"
#include "memory.h"

#define CPUID1 0x10
#define CPUID_VMX_BIT 0x20
#define IA32_FEATURE_CONTROL_LOCK_BIT 1 << 0
#define IA32_FEATURE_CONTROL_VMXON_INSIDE_SMX 1 << 1
#define IA32_FEATURE_CONTROL_VMXON_OUTSIDE_SMX 1 << 2
#define IA32_FEATURE_CONTROL 0x3a
#define IA32_VMX_BASIC 0x480
#define IA32_VMX_CR0_FIXED0 0x486
#define IA32_VMX_CR0_FIXED1 0x487
#define IA32_VMX_CR4_FIXED0 0x488
#define IA32_VMX_CR4_FIXED1 0x489


struct vmcs_t {
    union {
        uint64_t control;
        struct {
            uint64_t revision_identifier : 31;
            uint64_t shadow : 1;
        } bits;
    } header ;
    uint32_t abort_indicator;
    uint64_t data;
};

struct vmx {
    struct vmcs_t *vmcs_region;
    void *vmxon_region;
};

struct vcpu {
    struct vmx *vmx;
};

extern struct vcpu *current_cpu;

struct vcpu *current_cpu = {NULL};

void vmx_init() {
    union cr0_t cr0;
    union cr4_t cr4;
    uint32_t cpu_feature = cpuidf(CPUID1);
    uint64_t ia32_feature_control = read_msr(IA32_FEATURE_CONTROL);
    uint64_t ia32_vmx_basic = read_msr(IA32_VMX_BASIC);
    uint64_t cr00, cr01, cr40, cr41;

    cr00 = read_msr(IA32_VMX_CR0_FIXED0);
    cr01 = read_msr(IA32_VMX_CR0_FIXED1);
    cr40 = read_msr(IA32_VMX_CR4_FIXED0);
    cr41 = read_msr(IA32_VMX_CR4_FIXED1);

    cr0.control = read_cr0();
    cr4.control = read_cr4();

    cr0.control |= cr00;
    cr0.control &= cr01;
    write_cr0(cr0.control);
    cr4.control |= cr40;
    cr4.control &= cr41;
    write_cr4(cr4.control);

    /* check vmx is enable or not */
    send_serials("cpuid: ");
    log_u64((uint64_t)cpu_feature);

    if (cpu_feature & (1 << 6)) {
        log_char("SMX is usable");

        if (cr4.bits.smxe) {
            log_char("SMX is enable");
        }
    }

    send_serials("IA32_feature: ");
    log_u64(ia32_feature_control);
    if (cpu_feature & CPUID_VMX_BIT) {
        log_char("vmxe bit is set");
        if (ia32_feature_control & IA32_FEATURE_CONTROL_LOCK_BIT) {
            /* vmx is enable　*/
            log_char("lock bit is set");
            if (ia32_vmx_basic & ((uint64_t)1 << 48)) {
                log_char("vmx basic[bit48] is set");
            }

            if (ia32_feature_control & IA32_FEATURE_CONTROL_VMXON_INSIDE_SMX) {
                log_char("inside smx bit is set");
            }

            if (ia32_feature_control & IA32_FEATURE_CONTROL_VMXON_OUTSIDE_SMX) {
                log_char("outside smx bit is set");
            }
            
        }
    } else {
        put_s("vmx is not enable");
        return;
    }
    
    cr4.bits.vmxe = 1;
    cr4.bits.smxe = 0;
    write_cr4(cr4.control);
    log_char("write cr4");

    cr4.control = read_cr4();

    if (cr4.bits.smxe) {
        log_char("smx is enable");
    } else {
        log_char("smx is disable");
    }
 
    //vmxon_region = alloc_pages(2);
    //current_cpu->vmx->vmxon_region = vmxon_region;
    exec_vmxon((uint64_t)0x1000);
    log_char("vmxon");
    put_s("vmxon");
}