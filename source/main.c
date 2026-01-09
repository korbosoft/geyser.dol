/*
	geyser.dol by Korbosoft et al.
	(Generative Emulator Yielding Sequential Entropic Rectangles)

	TODO: licensing information
*/

#include <grrlib.h>

#include <ogc/lwp_watchdog.h>   // Needed for gettime and ticks_to_millisecs
#include <ogcsys.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <wiiuse/wpad.h>
#include <aesndlib.h>

#include "freemonobold_ttf.h"

// RGBA Colors
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

#define SAMPLE_RATE 48000
#define MIN_FREQ 440
#define MAX_FREQ 1760

// static u8 CalculateFrameRate(void);

static void VoiceCallBack(AESNDPB *pb, u32 state)
{
    if (state == VOICE_STATE_STOPPED)
        AESND_FreeVoice(pb);
}

static s16 *generate_sine(void) {
    double freq = ((double)rand() / (double)RAND_MAX) * (MAX_FREQ - MIN_FREQ) + MIN_FREQ;

    s16 *output_buffer = malloc(SAMPLE_RATE * 2);
    if (!output_buffer) {
        return NULL;
    }

    double phase = 0.0;
    double phase_step = (2.0 * M_PI * freq) / (double)SAMPLE_RATE;

    for (u16 i = 0; i < SAMPLE_RATE; i++) {
        output_buffer[i] = (s16)(sin(phase) * 32767);

        phase += phase_step;

        if (phase >= 2.0 * M_PI) {
            phase -= 2.0 * M_PI;
        }
    }

    return output_buffer;
}

static u32 rectColors[MAX_RECTANGLES];

static void get_rect_colors(u8 count);

typedef struct {
    f32 x, y, w, h;
} Rect;

static Rect random_rectangle(void) {
    const f32 x = (float)rand() / (float)RAND_MAX * rmode->fbWidth;
    const f32 y = (float)rand() / (float)RAND_MAX * rmode->efbHeight;
    return (Rect){
        x,
        y,
        ((float)rand() / (float)RAND_MAX) * (rmode->fbWidth - x),
        ((float)rand() / (float)RAND_MAX) * (rmode->efbHeight - y)
    };
}

static void delay(unsigned s) {
    unsigned long long start, end;
    start = gettime();
    while (1)
    {
        end = gettime();
        if (diff_sec(start,end) >= s)
            break;
    }
}

static u8 rectangleCount = 4;

int main() {
    unsigned long long start = gettime(), end = gettime();
    s16 slide = -1;

    AESND_Init();

    AESNDPB* voice;

    Rect rects[MAX_RECTANGLES];

    srand(time(0));

    GRRLIB_Init();

    GRRLIB_ttfFont *font = GRRLIB_LoadTTF(freemonobold_ttf, freemonobold_ttf_size);

    GRRLIB_SetBackgroundColour(255, 255, 255, 255);

    WPAD_Init();

    get_rect_colors(rectangleCount);

    GRRLIB_FillScreen(GRRLIB_WHITE);
    GRRLIB_Render();

    delay(1);
    s16 *soundBuf = generate_sine();
    voice = AESND_AllocateVoice(VoiceCallBack);
    AESND_PlayVoice(voice, VOICE_MONO16, soundBuf, SAMPLE_RATE * 2, SAMPLE_RATE, 0, false);

    while(1) {
        char str[25];
        if (diff_sec(start,end)) {
            // get_rect_colors(rectangleCount);

            if (slide == 9999) break;

            soundBuf = generate_sine();
            AESND_SetVoiceStop(voice, true);
            voice = AESND_AllocateVoice(VoiceCallBack);
            AESND_PlayVoice(voice, VOICE_MONO16, soundBuf, SAMPLE_RATE * 2, SAMPLE_RATE, 0, false);
            free(soundBuf);
            for (u8 i = 0; i < rectangleCount; i++) {
                rects[i] = random_rectangle();
            }

            start = gettime();
            end = gettime();

            slide++;
        } else end = gettime();
        WPAD_ScanPads();
        const u32 wpaddown = WPAD_ButtonsDown(0);
        // const u32 wpadheld = WPAD_ButtonsHeld(0);

        if (wpaddown & WPAD_BUTTON_HOME) break;

        GRRLIB_FillScreen(GRRLIB_WHITE);
        for (u8 i = 0; i < rectangleCount; i++) {
            GRRLIB_Rectangle(
                rects[i].x,
                rects[i].y,
                rects[i].w,
                rects[i].h,
                rectColors[i],
                true
            );
        };

        sprintf(str, "geyser.dol - slide %04u", slide);
        GRRLIB_PrintfTTF(16, rmode->efbHeight - 43, font, str, 11, GRRLIB_BLACK);

        GRRLIB_Render();
    }

    AESND_FreeVoice(voice);

    GRRLIB_FreeTTF(font);
    GRRLIB_Exit();
    exit(0);
    return 0;
}

static void get_rect_colors(u8 count) {
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

// static u8 CalculateFrameRate(void) {
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
