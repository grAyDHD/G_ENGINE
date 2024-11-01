#include "engine.h"

int main() {
    DSPC = (MODE3 | BG2);
    unsigned short green = 0x03E0;

    Coordinate start = { 20, 20 };

    drawRect(start, 80, 60, green, VRAM);
    return 0;
}
