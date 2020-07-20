#include "frameBuffer.h"
#include "window.h"
#include "graphics.h"
#include <stdint.h>

void vmmEntry(FrameBuffer *frameBuffer) {
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