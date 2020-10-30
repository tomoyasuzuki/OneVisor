#include <stdint.h>
#define vmmCS 1 << 3

void init_segment();

uint64_t createCS(uint64_t type, uint64_t dpl);
uint64_t createDS(uint64_t type, uint64_t dpl);