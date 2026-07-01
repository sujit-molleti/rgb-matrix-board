#include "LEDPixelBuffer.h"

LEDPixelBuffer::LEDPixelBuffer(int width, int height)
  : width(width),
    height(height),
    pixels(width * height, {0, 0, 0}) {
}

void LEDPixelBuffer::clear(LEDPixel pixel) {
  for (LEDPixel& currentPixel : pixels) {
    currentPixel = pixel;
  }
}

void LEDPixelBuffer::setPixel(int x, int y, LEDPixel pixel) {
  if (!contains(x, y)) {
    return;
  }

  pixels[pixelIndex(x, y)] = pixel;
}

LEDPixel LEDPixelBuffer::getPixel(int x, int y) const {
  if (!contains(x, y)) {
    return {0, 0, 0};
  }

  return pixels[pixelIndex(x, y)];
}

bool LEDPixelBuffer::contains(int x, int y) const {
  return x >= 0 && y >= 0 && x < width && y < height;
}

int LEDPixelBuffer::pixelIndex(int x, int y) const {
  return y * width + x;
}
