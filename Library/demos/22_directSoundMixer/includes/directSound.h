#ifndef DIRECTSOUND_H
#define DIRECTSOUND_H

#include "audio/audio.h"
#include "audio/mixer.h"

#include "core/dma.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "core/typedefs.h"

#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

#include "input/in.h"

#include "../includes/hat.h"
#include "../includes/k16.h"
#include "../includes/k8.h"
#include "../includes/s16.h"
#include "../includes/s8.h"
#include "../includes/tharp16.h"

void setMixChannels();

void startMusicLoop();
void stopMusicLoop();

void triggerKick();
void triggerSnare();
void triggerHat();

#endif
