#include "../build/MockSnek.h"
#include "../build/a.h"
#include "../build/b.h"
#include "../build/c.h"
#include "../build/d.h"
#include "../build/e.h"
#include "../build/f.h"
#include "../build/g.h"
#include "../build/h.h"
#include "../build/i.h"
#include "../build/j.h"
#include "../build/k.h"
#include "../build/l.h"
#include "../build/m.h"
#include "../build/n.h"
#include "../build/o.h"
#include "../build/p.h"
#include "../build/q.h"

#include "core/memory.h"
#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;
  const void *image[17] = {aBitmap, bBitmap, cBitmap, dBitmap, eBitmap, fBitmap,
                           gBitmap, hBitmap, iBitmap, jBitmap, kBitmap, lBitmap,
                           mBitmap, nBitmap, oBitmap, pBitmap, qBitmap};

  int currentImage = 0;
  while (1) {
    VBLANK();
    memCopy32(VRAM, image[currentImage], hBitmapLen / sizeof(u32));
    currentImage++;
    if (currentImage == 18) {
      currentImage = 0;
    }
    simpleWait(500);
  }

  return 0;
}
