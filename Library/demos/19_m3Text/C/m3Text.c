#include "../includes/fontData.h"
#include "draw.h"
#include "gfx.h"

int getFontDataIndex(char c);

void printString(int x, int y, const char *text);
void printDebug(int x, int y, const char *format, int arg1, int arg2);
void itoh(int number, char *buffer);

int main() {
  DSPC = MODE3 | BG2;
  int bgColor = dblClr(RGB(17, 13, 28));
  fillScreen(bgColor);

  // const char *text = "Hello World!";
  int x = 20;
  int y = 20;
  //  printString(x, y, text);

  printDebug(x, y, "pba = 0x%08x", (unsigned int)&PeaberryBitmap, 0);
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

void itoh(int number, char *buffer) {
  const char hexChars[] = "0123456789ABCDEF";    // Lookup array for hex digits
  for (int i = 0; i < 8; i++) {                  // Process 8 nibbles (32 bits)
    int nibble = (number >> (28 - i * 4)) & 0xF; // Extract nibble
    buffer[i] = hexChars[nibble];                // Convert to hex char
  }
  buffer[8] = '\0'; // Null-terminate the string
}

void printDebug(int x, int y, const char *format, int arg1, int arg2) {
  char stringOutput[64];
  int outputIndex = 0;
  int arg = 1;
  char formatBuffer[9];

  while (*format) {
    if (*format == '%') {
      format++;
      if (*format == 'x') {
        if (arg == 1) {
          itoh(arg1, formatBuffer);
          arg = 2;
        } else {
          itoh(arg2, formatBuffer);
        }
        for (int i = 0; formatBuffer[i] != '\0'; i++) {
          stringOutput[outputIndex++] = formatBuffer[i];
        }
        format++; // move past 'x'
      } else {
        // for now, add '%' to output and continue
        stringOutput[outputIndex++] = '%';
      }
    } else {

      stringOutput[outputIndex++] = *format++;
    }
  }

  stringOutput[outputIndex] = '\0'; // Null terminat output
  printString(x, y, stringOutput);  // Display the final string
}
