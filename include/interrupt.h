#pragma once
#include <stdint.h>

union interrupt_descriptor_attr {
    uint8_t data;
    struct {
        uint8_t descriptor_type : 3;
        uint8_t size : 1;
        uint8_t : 1;
        uint8_t dpl : 2;
        uint8_t present_flag : 1;
    } bits;
};

struct interrupt_descriptor {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    interrupt_descriptor_attr attr;
    uint16_t offset_high;
} interrupt_descriptor;

void init_idt();