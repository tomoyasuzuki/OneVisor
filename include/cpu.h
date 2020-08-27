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