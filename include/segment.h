#include <stdint.h>
#define vmmCS (1 << 3)
#define vmmSS (2 << 3)
#define ktss (5 << 3)

void init_segment();

uint64_t createCS(uint64_t type, uint64_t dpl);
uint64_t createDS(uint64_t type, uint64_t dpl);
void init_tss();