#include "../includes/22_audioMixer.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"


int main() {
    DSPC = MODE3 | BG2;
    gprintf(0, 0, "First sample: %d", guitarData[1]);
    gprintf(0, 40, "Last sample: %d", guitarData[AUDIO_LENGTH - 1]);

    return 0;
}
