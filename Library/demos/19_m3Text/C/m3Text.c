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

  const char *text = "Hello World!";
  int x = 20;
  int y = 20;
  int glyphIndex;

  for (int i = 0; text[i] != '\0'; i++) {
    char c = text[i];

    if (c == ' ') {
      x += 5;
      continue;
    }
    glyphIndex = getFontDataIndex(c);
    if (glyphIndex >= 0) {
      copyGlyphToVRAM(x, y, &PeaberryBitmap, &fontData[glyphIndex]);
      x += fontData[glyphIndex].width + 1;
    }
  }

  while (1) {
  }
  return 0;
}
