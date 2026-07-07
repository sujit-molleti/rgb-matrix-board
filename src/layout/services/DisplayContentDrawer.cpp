#include "DisplayContentDrawer.h"

DisplayContentDrawer::DisplayContentDrawer(DisplayLEDPixelBuffer& displayBuffer)
  : displayBuffer(displayBuffer) {
}

void DisplayContentDrawer::drawChar(
  int x,
  int y,
  char character,
  LEDPixel pixel
) {
  for (int row = 0; row < 5; row++) {
    unsigned char bits = glyphRow(character, row);

    for (int col = 0; col < 3; col++) {
      if (bits & (0b100 >> col)) {
        displayBuffer.setPixel(x + col, y + row, pixel);
      }
    }
  }
}

void DisplayContentDrawer::drawText(
  int x,
  int y,
  const char* text,
  LEDPixel pixel
) {
  for (int i = 0; text[i] != '\0'; i++) {
    drawChar(x + (i * 4), y, text[i], pixel);
  }
}

unsigned char DisplayContentDrawer::glyphRow(char character, int row) const {
  switch (character) {
    case '0': return (unsigned char[]){0b111, 0b101, 0b101, 0b101, 0b111}[row];
    case '1': return (unsigned char[]){0b010, 0b110, 0b010, 0b010, 0b111}[row];
    case '2': return (unsigned char[]){0b111, 0b001, 0b111, 0b100, 0b111}[row];
    case '3': return (unsigned char[]){0b111, 0b001, 0b111, 0b001, 0b111}[row];
    case '4': return (unsigned char[]){0b101, 0b101, 0b111, 0b001, 0b001}[row];
    case '5': return (unsigned char[]){0b111, 0b100, 0b111, 0b001, 0b111}[row];
    case '6': return (unsigned char[]){0b111, 0b100, 0b111, 0b101, 0b111}[row];
    case '7': return (unsigned char[]){0b111, 0b001, 0b010, 0b010, 0b010}[row];
    case '8': return (unsigned char[]){0b111, 0b101, 0b111, 0b101, 0b111}[row];
    case '9': return (unsigned char[]){0b111, 0b101, 0b111, 0b001, 0b111}[row];
    case 'A': return (unsigned char[]){0b010, 0b101, 0b111, 0b101, 0b101}[row];
    case 'B': return (unsigned char[]){0b110, 0b101, 0b110, 0b101, 0b110}[row];
    case 'E': return (unsigned char[]){0b111, 0b100, 0b110, 0b100, 0b111}[row];
    case 'J': return (unsigned char[]){0b001, 0b001, 0b001, 0b101, 0b111}[row];
    case 'L': return (unsigned char[]){0b100, 0b100, 0b100, 0b100, 0b111}[row];
    case 'N': return (unsigned char[]){0b101, 0b111, 0b111, 0b111, 0b101}[row];
    case 'P': return (unsigned char[]){0b110, 0b101, 0b110, 0b100, 0b100}[row];
    case 'Q': return (unsigned char[]){0b111, 0b101, 0b101, 0b111, 0b001}[row];
    case 'S': return (unsigned char[]){0b111, 0b100, 0b111, 0b001, 0b111}[row];
    case 'T': return (unsigned char[]){0b111, 0b010, 0b010, 0b010, 0b010}[row];
    case 'Y': return (unsigned char[]){0b101, 0b101, 0b010, 0b010, 0b010}[row];
    case ':': return (unsigned char[]){0b000, 0b010, 0b000, 0b010, 0b000}[row];
    case '>': return (unsigned char[]){0b100, 0b010, 0b001, 0b010, 0b100}[row];
    case ' ': return 0b000;
    default: return 0b000;
  }
}
