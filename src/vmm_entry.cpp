#include "frameBuffer.hpp"
#include "window.hpp"
#include "graphics.hpp"
#include "assm.h"
#include <stdint.h>

extern "C" void vmmEntry(FrameBuffer *frameBuffer) {
    Window window = {
        frameBuffer->baseAddr,
        frameBuffer->size, 
        frameBuffer->width, 
        frameBuffer->height, 
        frameBuffer->pixelFormat,
        frameBuffer->pixelsPerScanLine
    };
    
    for (uint64_t i = 0; i < window.size; i++) {
      window.baseAddr[i] = 0xffffffff;
  }
}