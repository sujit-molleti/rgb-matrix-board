#include "DisplayLEDPixelBuffer.h"

DisplayLEDPixelBuffer::DisplayLEDPixelBuffer(int width, int height)
  : width(width),
    height(height),
    pixels(width * height, {0, 0, 0}) {
}

int DisplayLEDPixelBuffer::getWidth() const {
  return width;
}

int DisplayLEDPixelBuffer::getHeight() const {
  return height;
}

void DisplayLEDPixelBuffer::clear(LEDPixel pixel) {
  for (LEDPixel& currentPixel : pixels) {
    currentPixel = pixel;
  }
}

void DisplayLEDPixelBuffer::setPixel(int x, int y, LEDPixel pixel) {
  if (!contains(x, y)) {
    return;
  }

  pixels[pixelIndex(x, y)] = pixel;
}

LEDPixel DisplayLEDPixelBuffer::getPixel(int x, int y) const {
  if (!contains(x, y)) {
    return {0, 0, 0};
  }

  return pixels[pixelIndex(x, y)];
}

bool DisplayLEDPixelBuffer::contains(int x, int y) const {
  return x >= 0 && y >= 0 && x < width && y < height;
}

int DisplayLEDPixelBuffer::pixelIndex(int x, int y) const {
  return y * width + x;
}
