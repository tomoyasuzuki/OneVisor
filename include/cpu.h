#pragma once
#include <stdint.h>


union cr0_t {
    uint64_t control;
    struct {
        uint64_t pe : 1;
        uint64_t mp : 1;
        uint64_t em : 1;
        uint64_t ts : 1;
        uint64_t et : 1;
        uint64_t ne : 1;
        uint64_t reserved1 : 10;
        uint64_t wp : 1;
        uint64_t reserved2 : 1;
        uint64_t am : 1;
        uint64_t reserved3 : 10;
        uint64_t nw : 1;
        uint64_t cd : 1;
        uint64_t pg : 1;
        uint64_t reserved4 : 32;
    } bits;
};

union cr4_t {
    uint64_t control;
    struct {
        uint64_t vme : 1;
        uint64_t pvi : 1;
        uint64_t tsd : 1;
        uint64_t de : 1;
        uint64_t pse : 1;
        uint64_t pae : 1;
        uint64_t mce : 1;
        uint64_t pge : 1;
        uint64_t pce : 1;
        uint64_t osfxsr : 1;
        uint64_t osxmmexcpt : 1;
        uint64_t umip : 1;
        uint64_t : 1;
        uint64_t vmxe : 1;
        uint64_t smxe : 1;
        uint64_t fsgsbase : 1;
        uint64_t : 1;
        uint64_t pcide : 1;
        uint64_t osxsave : 1;
        uint64_t : 1;
        uint64_t smep : 1;
        uint64_t smap : 1;
        uint64_t : 42;      
    } bits;
};

union segment_descriptor {
    uint64_t control;
    struct {
        uint64_t limit : 16;
        uint64_t base1 : 16;
        uint64_t base2 : 8;
        uint64_t type : 4;
        uint64_t s : 1;
        uint64_t dpl : 2;
        uint64_t p : 1;
        uint64_t seg_limit : 4;
        uint64_t avl : 1;
        uint64_t l : 1;
        uint64_t db : 1;
        uint32_t g : 1;
        uint64_t base3 : 8;
    } bits;
};

struct vmcs_t {
    union {
        u_int64_t control;
        struct {
            uint64_t revision_identifier : 31;
            uint64_t shadow : 1;
        } bits;
    } header ;
    uint32_t abort_indicator;
    uint64_t data;
};

union vmxon_region {
    uint32_t control;
    struct {
        uint32_t revision_identifier : 32;
        uint32_t data : 32;
    } bits;
};