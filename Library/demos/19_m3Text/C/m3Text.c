#include "../includes/m3Text.h"
#include "draw.h"
#include "gfx.h"

int getFontDataIndex(char c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  } else if (c >= 'a' && c <= 'z') {
    return c - 'a' + 26;
  } else if (c >= '0' && c <= '9') {
    return c - '0' + 52;
  } else {
    switch (c) {
    case '{':
      return 62;
    case '}':
      return 63;
    case '(':
      return 64;
    case ')':
      return 65;
    case '[':
      return 66;
    case ']':
      return 67;
    case '<':
      return 68;
    case '>':
      return 69;
    case '$':
      return 70;
    case '#':
      return 71;
    case '@':
      return 72;
    case '+':
      return 73;
    case '-':
      return 74;
    case '*':
      return 75;
    case '/':
      return 76;
    case '^':
      return 77;
    case '=':
      return 78;
    case '%':
      return 79;
    case '&':
      return 80;
    case '\\':
      return 81;
    case '|':
      return 82;
    case '~':
      return 83;
    case '!':
      return 84;
    case '?':
      return 85;
    case '\'':
      return 86;
    case '\"':
      return 87;
    case '.':
      return 88;
    case ',':
      return 89;
    case ';':
      return 90;
    case ':':
      return 91;
    case '`':
      return 92;
    case '_':
      return 93;
    default:
      return -1;
    }
  }
}

void drawGlyph(u8 x, u8 y, char c) {
  int index = getFontDataIndex(c);
  if (index < 0) {
    return;
  }

  GlyphInfo glyph = fontData[index];
  int glyphOffset = glyph.xOffset + (glyph.yOffset * 803);
  //   void *glyphImagePtr = &PeaberryBitmap[glyphOffset];

  //  copyGlyphToVRAM(x+xoffset, y+yoffset, glyphImagePtr, &glyph);
}

int main() {
  DSPC = MODE3 | BG2;

  return 0;
}
