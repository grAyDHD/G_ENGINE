#include "audio.h"
#include "typedefs.h"

void PlayNote(u16 frequency, unsigned char length) {
    unsigned char adjustedLength = 0xFF - length;

    REG_SOUNDCNT_X = 0x80;
    REG_SOUNDCNT_L = 0xFF77;
    REG_SOUNDCNT_H = 2;

    REG_SOUND3CNT_L = SOUND3BANK32 | SOUND3SETBANK1;
    REG_WAVE_RAM0 = 0x10325476;
    REG_WAVE_RAM1 = 0x98badcfe;
    REG_WAVE_RAM2 = 0x10325476;
    REG_WAVE_RAM3 = 0x98badcfe;
    REG_SOUND3CNT_L = SOUND3BANK32 | SOUND3SETBANK0;

    REG_SOUND3CNT_L |= SOUND3PLAY;
    REG_SOUND3CNT_H = SOUND3OUTPUT1 | adjustedLength;
    REG_SOUND3CNT_X = SOUND3INIT | SOUND3PLAYONCE | frequency;
}
