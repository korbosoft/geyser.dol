#ifndef VIDEO_H
#define VIDEO_H

// RGBA Colors
// we should probably rename these from GRRLIB_* to something
// more generic. how about COLOR_*
#define GRRLIB_BLACK   0x000000FF
#define GRRLIB_MAROON  0x800000FF
#define GRRLIB_GREEN   0x008000FF
#define GRRLIB_OLIVE   0x808000FF
#define GRRLIB_NAVY    0x000080FF
#define GRRLIB_PURPLE  0x800080FF
#define GRRLIB_TEAL    0x008080FF
#define GRRLIB_GRAY    0x808080FF
#define GRRLIB_SILVER  0xC0C0C0FF
#define GRRLIB_RED     0xFF0000FF
#define GRRLIB_LIME    0x00FF00FF
#define GRRLIB_YELLOW  0xFFFF00FF
#define GRRLIB_BLUE    0x0000FFFF
#define GRRLIB_FUCHSIA 0xFF00FFFF
#define GRRLIB_AQUA    0x00FFFFFF
#define GRRLIB_WHITE   0xFFFFFFFF

#define MAX_RECTANGLES 12

extern u32 rectColors[MAX_RECTANGLES];

typedef struct {
    f32 x, y, w, h;
} Rect;

Rect random_rectangle(int maxWidth, int maxHeight);
void get_rect_colors(u8 count);
// u8 CalculateFrameRate(void);

#endif // VIDEO_H
