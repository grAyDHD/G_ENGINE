#include "../includes/fontData.h"
#include "draw.h"
#include "gfx.h"

#define dblClr(color) ((color) | (color << 16))

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
int main() {
  DSPC = MODE3 | BG2;
  int bgColor = dblClr(RGB(17, 13, 28));
  fillScreen(bgColor);
  int glyphIndex;

  glyphIndex = getFontDataIndex('H');
  copyGlyphToVRAM(20, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('e');
  copyGlyphToVRAM(30, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('l');
  copyGlyphToVRAM(40, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('l');
  copyGlyphToVRAM(50, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('o');
  copyGlyphToVRAM(60, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('W');
  copyGlyphToVRAM(80, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('o');
  copyGlyphToVRAM(90, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('r');
  copyGlyphToVRAM(100, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('l');
  copyGlyphToVRAM(110, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('d');
  copyGlyphToVRAM(120, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  glyphIndex = getFontDataIndex('!');
  copyGlyphToVRAM(130, 20, &PeaberryBitmap, &fontData[glyphIndex]);

  while (1) {
  }
  return 0;
}

/*
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
*/
