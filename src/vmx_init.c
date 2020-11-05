#include "vmx_init.h"
#include "cpu.h"
#include "assembly.h"
#include "window.h"
#include "serial.h"
#include "memory.h"
#include <stdalign.h>

#define CPUID1 0x1
#define CPUID_VMX_BIT 0x20
#define IA32_FEATURE_CONTROL_LOCK_BIT (1 << 0)
#define IA32_FEATURE_CONTROL_VMXON_INSIDE_SMX (1 << 1)
#define IA32_FEATURE_CONTROL_VMXON_OUTSIDE_SMX (1 << 2)
#define IA32_FEATURE_CONTROL 0x3a
#define IA32_VMX_BASIC 0x480
#define IA32_VMX_CR0_FIXED0 0x486
#define IA32_VMX_CR0_FIXED1 0x487
#define IA32_VMX_CR4_FIXED0 0x488
#define IA32_VMX_CR4_FIXED1 0x489

union ia32_vmx_basic {
    uint64_t control;
    struct {
        uint64_t revision_identifier : 32;
        uint64_t vmxon_region_size : 13;
        uint64_t : 3;
        uint64_t vmxon_physi_width : 1;
        uint64_t dual_monitor_smi : 1;
        uint64_t memory_type : 4;
        uint64_t io : 1;
        uint64_t true_control : 1;

    } bits;
};

struct vmx {
    struct vmcs_t *vmcs_region;
    void *vmxon_region;
};

struct vmxon_region {
    uint32_t id;
    uint64_t data;
} __attribute__((packed));

struct vcpu {
    struct vmx *vmx;
};

extern struct vcpu *current_cpu;

struct vcpu *current_cpu = {NULL};

void vmx_init() {
    union cr0_t cr0;
    union cr4_t cr4;
    uint64_t cr00, cr01, cr40, cr41;
    union ia32_vmx_basic b;
    uint32_t cpu_feature = cpuidf(CPUID1);
    uint64_t ia32_feature_control = read_msr(IA32_FEATURE_CONTROL);
    struct vmxon_region *vmxon = NULL;

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

    send_serials("cr00: ");
    log_u64(cr00);
    send_serials("cr01: ");
    log_u64(cr01);
    send_serials("cr40: ");
    log_u64(cr40);
    send_serials("cr41: ");
    log_u64(cr41);
    send_serials("cr0: ");
    log_u64(cr0.control);
    send_serials("cr4: ");
    log_u64(cr4.control);


    b.control = read_msr(IA32_VMX_BASIC);
    log_u64(b.control);

    /* check vmx is enable or not */
    send_serials("cpuid: ");
    log_u64((uint64_t)cpu_feature);

    send_serials("IA32_feature: ");
    log_u64(ia32_feature_control);

    if (cpu_feature & CPUID_VMX_BIT) {
        log_char("vmxe bit is set");

        if (ia32_feature_control & IA32_FEATURE_CONTROL_LOCK_BIT) {
            /* vmx is enable　*/
            log_char("lock bit is set");

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
    char *hoge = (char*)alloc_page();
    log_char(hoge);
    vmxon = (struct vmxon_region*)alloc_page();
    vmxon->id = b.control;
    vmxon->data = 0;

    uint64_t f = read_flags();
    send_serials("f: ");
    log_u64(f);

    send_serials("IA32_VMX_BASIC: ");
    log_u64(b.bits.revision_identifier);
    send_serials("vmxon pointer: ");
    log_u64((uint64_t)vmxon);

    uint64_t result = exec_vmxon((uint64_t)vmxon);

    if (result) {
        log_char("Success");
    } else {
        log_char("Failed");
    }

    send_serials("result: ");
    log_u64(result);

    uint8_t cf = check_cf();

    if (cf == 1) {
        put_s("cf");
    } 

    uint8_t zf = check_zf();

    if (zf == 1) {
        put_s("zf");
    }

    uint8_t pf = check_pf();

    if (pf == 1) {
        put_s("pf");
    }

    uint8_t sf = check_sf();

    if (sf == 1) {
        put_s("sf is zero");
    }

    uint64_t flags = read_flags();

    log_u64(cf);
    log_u64(zf);
    log_u64(flags);

    exec_vmxoff();
    while(1);
    return;
}