#pragma once
#include "frameBuffer.h"
#include "memorymap.h"

typedef struct BootInfo {
    FrameBuffer *frame_buff;
    MemoryMap *memmap;
} BootInfo;