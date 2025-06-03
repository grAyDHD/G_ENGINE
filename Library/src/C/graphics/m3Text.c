//#include "assets/fonts/Peaberry/Peaberry.h"
//#include "assets/fonts/Peaberry/PeaberryData.h"

#include "assets/fonts/MiniGBA/MiniGBA.h"
#include "assets/fonts/MiniGBA/MiniGBAData.h"
#include "graphics/draw.h"

// upcoming features: variable text size, current font sctruct/pointer, can change to another font at whim. Use library to only include fonts directly used by given project in compilation. text color change if font is monocolor, with shadow settings?

int getFontDataIndex(char c);

void renderChar(int *x, int *y, char c);
void printString(int x, int y, const char *text);
void gprintf(int x, int y, const char *format, u32 arg);

static u16 currentTextColor = 0xFFFF;

void setTextColor(int r, int g, int b) {
  currentTextColor = RGB(r, g, b);
}

void gprintf(int x, int y, const char *fmt, u32 arg) {
  int started = 0;
  int counter;
  int powers[] = {10000, 1000, 100, 10, 1};

  s32 val = arg;
  for (int i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      i++;
      switch (fmt[i]) {
      case ('d'): 
        if (val == 0) {
          renderChar(&x, &y, '0');
          continue;
        }
        if (val < 0) {
          renderChar(&x, &y, '-');
          val = -val;
        }
        for (int j = 0; j < 5; j++) {
          counter = 0;
          while (val >= powers[j]) {
            val -= powers[j];
            counter++;
          }
          if (counter | started) {
            started = 1;
            renderChar(&x, &y, '0' + counter);
          }
        }
        break;

        case ('x'):
          renderChar(&x, &y, '0');
          renderChar(&x, &y, 'x');
          int nibble;
          for (int i = 7; i >= 0; i--) {
            nibble = ((arg >> (i * 4)) & 0xF);
            if (nibble < 10) {
              renderChar(&x, &y, '0' + nibble);
            } else {
              renderChar(&x, &y, 'A' + (nibble - 10));
            }
          }
          break;
        default:
          renderChar(&x, &y, '%');
          renderChar(&x, &y, fmt[i]);
        break;
      }
    } else {
      renderChar(&x, &y, fmt[i]);
    }
  }
}

void printString(int x, int y, const char *text) {
  for (int i = 0; text[i] != '\0'; i++) {
    renderChar(&x, &y, text[i]);
  }
};

void renderChar(int *x, int *y, char c) {
  if (c == ' ') {
    *x += 2;
    return;
  }

  int glyphIndex = getFontDataIndex(c);
  printGlyphColored(*x, *y, &MiniGBABitmap, &fontData[glyphIndex], currentTextColor);

  *x += fontData[glyphIndex].width + 1; 
};

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


