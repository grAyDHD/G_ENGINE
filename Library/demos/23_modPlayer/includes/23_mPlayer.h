#ifndef mplayer_H
#define mplayer_H

#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

#include "core/interrupts.h"
#include "core/typedefs.h"
#include "ecs/components.h"
#include "ecs/systems.h"
#include "graphics/video.h"

#include "../build/Bedroom.h"
#include "../build/Sonic.h"
#include "core/timer.h"
#include "ecs/ecs.h"
#include "graphics/video.h"

void irqDispatch();
// #include "audio/modPlayer.h"

#endif // !mplayer_H
