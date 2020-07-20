#pragma once
#include <stdint.h>

enum PixelFormat {
    kPixelRGBResv8BitPerColor,
    kPixelBGRResv8BitPerColor
};

typedef struct FrameBuffer {
    uint64_t* baseAddr;
    uint64_t size;
    uint32_t width;
    uint32_t height;
    uint32_t pixelsPerScanLine;
    enum PixelFormat pixelFormat;
}FrameBuffer;
