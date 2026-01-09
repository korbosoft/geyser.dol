#include <stdlib.h>
#include <math.h>
#include <ogcsys.h>
#include <aesndlib.h>

#include "sound.h"

void VoiceCallBack(AESNDPB *pb, u32 state)
{
    if (state == VOICE_STATE_STOPPED)
        AESND_FreeVoice(pb);
}

s16 *generate_sine(void) {
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

