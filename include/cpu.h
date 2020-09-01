#include <stdint.h>

union cr0 {
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