#include "engine.h"

u16 keyCache;
int main() {
    DSPC = MODE3 | BG2;

    Coordinate start = { 50, 60 };
    int rWidth = 40;
    int rHeight = 80;

    unsigned short colors[] = {
        0x001F, 0x03E0, 0x7C00, 0x7FFF, 0x03FF, 0x7FE0, 0x7C1F, 0x4631,
        0x1CE7, 0x021F, 0x03E7, 0x7C63, 0x7FFF, 0x1F1F, 0x03A0, 0x7C00
    };

    int cI = 0;
    unsigned short COLOR = colors[cI];
    while (1) {
        drawRect(start, rWidth, rHeight, 0x0000, VRAM);
        UPDATE_KEYS();

        if (INPUT(A)) {
            start.x = 50;
            start.y = 60;
            rWidth = 20;
            rHeight = 20;
        }
        if (INPUT(U)) {
            start.y--;
        }
        if (INPUT(D)) {
            start.y++;
        }
        if (INPUT(L)) {
            start.x--;
        }
        if (INPUT(R)) {
            start.x++;
        }
        if (INPUT(L | R)) {
            rWidth = 20;
            rHeight = 20;
        } else if (INPUT(L)) {
            rWidth++;
        } else if (INPUT(R)) {
            rHeight++;
        }

        // Change color if START button is pressed
        if (INPUT(ST)) {
            if (cI >= 14) {
                cI = 0;
            } else {
                cI++;
            }
            COLOR = colors[cI];
        }
        drawRect(start, rWidth, rHeight, COLOR, VRAM);
        waitVBLANK();
    }
    return 0;
}
