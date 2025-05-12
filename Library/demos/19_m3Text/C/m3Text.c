#include "../build/Peaberry.h"
#include "../includes/fontData.h"
#include "graphics/draw.h"
#include "graphics/video.h"

int getFontDataIndex(char c);
void printString(int x, int y, const char *text);
void gprintf(int x, int y, const char* format, int arg);

int main() {
  DSPC = MODE3 | BG2;
  int bgColor = dblClr(RGB(17, 13, 28));
  fillScreen(bgColor);

  const char *text = "Hello World!";
  int x = 20;
  int y = 20;
  printString(x, y, "0123");
//  gprintf(x, y, "hello %d", 0);
  while (1) {
  }
  return 0;
}

void printString(int x, int y, const char *text) {
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
};

//printing an int value, if < 0 and > -10, print '-' and digit
//if > 0 < 10, print digit
//if > 9 < 100, print 2 digits, etc.

void gprintf(int x, int y, const char *fmt, int arg) {
  int glyphIndex;
  char c;
  char buf[8] = {0};
  int counter;
  int powers[] = {10000, 1000, 100, 10, 1};
  int leadingZero = 0; 

  for (int i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      i++;

      switch (fmt[i]) {
      case ('d'): 

        if (arg == 0) {
            // Special case for zero
          buf[0] = '1';
          glyphIndex = getFontDataIndex('0');
          if (glyphIndex >= 0) {
            copyGlyphToVRAM(x, y, &PeaberryBitmap, &fontData[glyphIndex]);
            x += fontData[glyphIndex].width + 1;
          }

        } else if (arg < 0) {
          c = '-';
          glyphIndex = getFontDataIndex(c);
          if (glyphIndex >= 0) {
            copyGlyphToVRAM(x, y, &PeaberryBitmap, &fontData[glyphIndex]);
            x += fontData[glyphIndex].width + 1;
          }
          arg = -arg;

          } else {
            for (int j = 0; j < 5; j++) {
              counter = 0;
              while (arg > powers[j]) {
                arg -= powers[j];
                counter++;
              }
              if (!(counter | buf[0])) {
                leadingZero++;
              } else {
                buf[j - leadingZero] = '0' + counter;
              }
            }

            for (int j = 0; j < (5 - leadingZero); j++) {
                glyphIndex = getFontDataIndex(buf[j]);
                if (glyphIndex >= 0) {
                  copyGlyphToVRAM(x, y, &PeaberryBitmap, &fontData[glyphIndex]);
                  x += fontData[glyphIndex].width + 1;
              }
            }
          }

          i++;
          break;

        case ('x'):
        
           // i++
          break;
        default:
          c = '%';
          glyphIndex = getFontDataIndex(c);
          if (glyphIndex >= 0) {
            copyGlyphToVRAM(x, y, &PeaberryBitmap, &fontData[glyphIndex]);
            x += fontData[glyphIndex].width + 1;
          }
          break;
      }
    }

    char c = fmt[i];
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
}


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

