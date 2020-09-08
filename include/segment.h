#include <stdint.h>
#define vmmCS 1 << 3

void init_segment();

uint64_t create_segment_descriptor(uint64_t type, uint64_t dpl);