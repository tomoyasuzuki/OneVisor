#include "frameBuffer.h"
#include "window.h"
#include <stdint.h>

void vmmEntry(FrameBuffer *frameBuffer) {
    
    init_graphic((Window*)frameBuffer);
    blackout_window();
    draw_pixel(500, 500);
}