#ifndef SOUND_H
#define SOUND_H

#include <aesndlib.h>

#define SAMPLE_RATE 48000
#define MIN_FREQ 440
#define MAX_FREQ 1760

void VoiceCallBack(AESNDPB *pb, u32 state);
s16 *generate_sine(void);

#endif // SOUND_H
