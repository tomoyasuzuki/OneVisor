#include "frameBuffer.h"
#include "window.h"
#include <stdint.h>

void vmmEntry(FrameBuffer *frameBuffer) {
    
    init_graphic(frameBuffer);
    blackout_window();
    
    char *s = "OneVisor";
    put_s(s);
}