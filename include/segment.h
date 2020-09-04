#include <stdint.h>

void init_segment();

uint64_t create_segment_descriptor(uint64_t type, uint64_t dpl);