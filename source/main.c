/*===========================================
        GRRLIB (GX Version)
        - Example Code -

        How To use Bitmap Fonts
============================================*/
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

#define RECTANGLE_COUNT 2

#define SAMPLE_RATE 48000
#define RENDER_RATE SAMPLE_RATE * 2
#define MIN_FREQ 440
#define MAX_FREQ 1760

#define VOICE_RUNNING			0x40000000

struct aesndpb_t
{
    u32 out_buf;				//0

    u32 buf_start;				//4
    u32 buf_end;				//8
    u32 buf_curr;				//12

    u16 yn1;					//16
    u16 yn2;					//18
    u16 pds;					//20

    u16 freq_h;					//22
    u16 freq_l;					//24
    u16 counter;				//26

    s16 left,right;				//28,30
    u16 volume_l,volume_r;		//32,34

    u32 delay;					//36

    u32 flags;					//40

    u8 _pad[20];

    u32 mram_start;
    u32 mram_curr;
    u32 mram_end;
    u32 stream_last;

    u32 voiceno;
    u32 shift;
    AESNDVoiceCallbackArg cb;
    void *cbArg;

    AESNDAudioCallbackArg audioCB;
    void *audioCBArg;
};

// static u8 CalculateFrameRate(void);


static void VoiceCallBack(AESNDPB *pb, u32 state)
{
    if (state == VOICE_STATE_STOPPED)
        AESND_FreeVoice(pb);
}


s16 *generate_sine(void) {
    double freq = ((double)rand() / (double)RAND_MAX) * (MAX_FREQ - MIN_FREQ) + MIN_FREQ;
    printf("%f", freq);

    s16 *output_buffer = malloc(SAMPLE_RATE * 2);
    if (!output_buffer) {
        return NULL;
    }

    double phase = 0.0;
    double phase_step = (2.0 * M_PI * freq) / (double)SAMPLE_RATE;

    for (int i = 0; i < SAMPLE_RATE; i++) {
        output_buffer[i] = (s16)(sin(phase) * 32767);

        phase += phase_step;

        if (phase >= 2.0 * M_PI) {
            phase -= 2.0 * M_PI;
        }
        // printf("%i", output_buffer[i]);
    }

    return output_buffer;
}

const u32 rectColor[RECTANGLE_COUNT] = {
    GRRLIB_RED,
    GRRLIB_BLUE
};

typedef struct {
    f32 x, y, w, h;
} Rect;

Rect random_rectangle(void) {
    const f32 x = (float)rand() / (float)RAND_MAX * rmode->fbWidth;
    const f32 y = (float)rand() / (float)RAND_MAX * rmode->efbHeight;
    return (Rect){
        x,
        y,
        ((float)rand() / (float)RAND_MAX) * (rmode->fbWidth - x),
        ((float)rand() / (float)RAND_MAX) * (rmode->efbHeight - y)
    };
}

void delay(unsigned s) {
    unsigned long long start, end;
    start = gettime();
    while (1)
    {
        end = gettime();
        if (diff_sec(start,end) >= s)
            break;
    }
}

int main() {
    unsigned long long start = gettime(), end = gettime();
    s16 slide = -1;
    // u8 FPS = 0;

    AESND_Init();

    AESNDPB* voice;

    Rect rects[RECTANGLE_COUNT];

    srand(time(0));

    GRRLIB_Init();

    GRRLIB_ttfFont *font = GRRLIB_LoadTTF(freemonobold_ttf, freemonobold_ttf_size);

    GRRLIB_SetBackgroundColour(255, 255, 255, 255);

    WPAD_Init();
    // WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    delay(1);
    s16 *soundBuf = generate_sine();
    voice = AESND_AllocateVoice(VoiceCallBack);
    AESND_PlayVoice(voice, VOICE_MONO16, soundBuf, SAMPLE_RATE * 2, SAMPLE_RATE, 0, false);
    while(1) {
        char str[25];
        if (diff_sec(start,end)) {
            if (slide == 9999) break;

            soundBuf = generate_sine();
            AESND_SetVoiceStop(voice, true);
            voice = AESND_AllocateVoice(VoiceCallBack);
            AESND_PlayVoice(voice, VOICE_MONO16, soundBuf, SAMPLE_RATE * 2, SAMPLE_RATE, 0, false);
            free(soundBuf);
            rects[0] = random_rectangle();
            rects[1] = random_rectangle();

            start = gettime();
            end = gettime();

            slide++;
        } else end = gettime();
        // WPAD_SetVRes(0, 640, 480);
        WPAD_ScanPads();
        const u32 wpaddown = WPAD_ButtonsDown(0);
        // const u32 wpadheld = WPAD_ButtonsHeld(0);

        if (wpaddown & WPAD_BUTTON_HOME) break;
        // WPAD_IR(WPAD_CHAN_0, &ir1);

        GRRLIB_FillScreen(GRRLIB_WHITE);    // Clear the screen
        for (u8 i = 0; i < RECTANGLE_COUNT; i++) {
            GRRLIB_Rectangle(
                rects[i].x,
                rects[i].y,
                rects[i].w,
                rects[i].h,
                rectColor[i],
                true
            );
        };

        sprintf(str, "geyser.dol - slide %04u", slide);
        GRRLIB_PrintfTTF(16, rmode->efbHeight - 48, font, str, 16, GRRLIB_BLACK);

        GRRLIB_Render();
    }

    AESND_FreeVoice(voice);

    GRRLIB_FreeTTF(font);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    exit(0);
    return 0;
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
