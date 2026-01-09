#include <stdlib.h>
#include <ogcsys.h>

#include "video.h"

u32 rectColors[MAX_RECTANGLES];

Rect random_rectangle(int maxWidth, int maxHeight) {
    const f32 x = (float)rand() / (float)RAND_MAX * maxWidth;
    const f32 y = (float)rand() / (float)RAND_MAX * maxHeight;
    return (Rect){
        x,
        y,
        ((float)rand() / (float)RAND_MAX) * (maxWidth - x),
        ((float)rand() / (float)RAND_MAX) * (maxHeight - y)
    };
}

void get_rect_colors(u8 count) {
    rectColors[0] = GRRLIB_BLUE;
    switch (count) {
        case 2:
            rectColors[1] = GRRLIB_RED;
            break;
        case 3:
            rectColors[1] = GRRLIB_LIME;
            rectColors[2] = GRRLIB_RED;
            break;
        case 4:
            rectColors[1] = GRRLIB_LIME;
            rectColors[2] = GRRLIB_YELLOW;
            rectColors[3] = GRRLIB_RED;
            break;
        case 5:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            break;
        case 6:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            break;
        case 7:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            rectColors[6] = GRRLIB_NAVY;
            break;
        case 8:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            rectColors[6] = GRRLIB_NAVY;
            rectColors[7] = GRRLIB_MAROON;
            break;
        case 9:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            rectColors[6] = GRRLIB_NAVY;
            rectColors[7] = GRRLIB_GREEN;
            rectColors[8] = GRRLIB_MAROON;
            break;
        case 10:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            rectColors[6] = GRRLIB_NAVY;
            rectColors[7] = GRRLIB_GREEN;
            rectColors[8] = GRRLIB_OLIVE;
            rectColors[9] = GRRLIB_MAROON;
            break;
        case 11:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            rectColors[6] = GRRLIB_TEAL;
            rectColors[7] = GRRLIB_NAVY;
            rectColors[8] = GRRLIB_GREEN;
            rectColors[9] = GRRLIB_OLIVE;
            rectColors[10] = GRRLIB_MAROON;
            break;
        case 12:
            rectColors[1] = GRRLIB_AQUA;
            rectColors[2] = GRRLIB_LIME;
            rectColors[3] = GRRLIB_YELLOW;
            rectColors[4] = GRRLIB_RED;
            rectColors[5] = GRRLIB_FUCHSIA;
            rectColors[6] = GRRLIB_TEAL;
            rectColors[7] = GRRLIB_NAVY;
            rectColors[8] = GRRLIB_GREEN;
            rectColors[9] = GRRLIB_OLIVE;
            rectColors[10] = GRRLIB_MAROON;
            rectColors[11] = GRRLIB_PURPLE;
            break;
    }
}

// u8 CalculateFrameRate(void) {
//     static u8 frameCount = 0;
//     static u32 lastTime;
//     static u8 FPS = 0;
//     const u32 currentTime = ticks_to_millisecs(gettime());
//
//     frameCount++;
//     if(currentTime - lastTime > 1000) {
//         lastTime = currentTime;
//         FPS = frameCount;
//         frameCount = 0;
//     }
//     return FPS;
// }
