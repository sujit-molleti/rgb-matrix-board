#include "LayoutRenderer.h"

#include "DisplayContentDrawer.h"
#include "LayoutConfigLoader.h"
#include "LayoutValidator.h"
#include "../internal/layout_private.h"
#include <generated/LayoutConfigJson.h>

namespace {
const LEDPixel White = {255, 255, 255};

const Layout* findLayoutById(const VirtualRgbBoard& board, const std::string& layoutId) {
  for (const Layout& layout : board.layouts) {
    if (layout.id == layoutId) {
      return &layout;
    }
  }

  return nullptr;
}

const Display* findDisplayById(const Layout& layout, const char* displayId) {
  for (const Display& display : layout.displays) {
    if (display.id == displayId) {
      return &display;
    }
  }

  return nullptr;
}
}

LayoutRenderer::LayoutRenderer(int boardWidth, int boardHeight)
  : board(LayoutConfigLoader().load(layout_config::kBoardLayoutsJson)),
    currentLayoutId(board.defaultLayoutId),
    pixelBuffer(boardWidth, boardHeight) {
}

void LayoutRenderer::clear(LEDPixel pixel) {
  pixelBuffer.clear(pixel);
}

void LayoutRenderer::renderDisplayBorders() {
  const Layout* layout = activeLayout();

  if (layout == nullptr) {
    return;
  }

  for (const Display& display : layout->displays) {
    DisplayLEDPixelBuffer displayBuffer = displayLEDPixelBufferFor(display);
    DisplayContentDrawer drawer(displayBuffer);
    drawer.drawText(2, 2, "Hello", White);
    renderDisplay(display.id.c_str(), displayBuffer);
  }
}

void LayoutRenderer::setPixel(int x, int y, LEDPixel pixel) {
  pixelBuffer.setPixel(x, y, pixel);
}

void LayoutRenderer::renderDisplay(
  const char* displayId,
  const DisplayLEDPixelBuffer& displayBuffer
) {
  const Layout* layout = activeLayout();

  if (layout == nullptr) {
    return;
  }

  const Display* display = findDisplayById(*layout, displayId);

  if (display == nullptr ||
      displayBuffer.getWidth() != display->bounds.width ||
      displayBuffer.getHeight() != display->bounds.height) {
    return;
  }

  for (int y = 0; y < display->bounds.height; y++) {
    for (int x = 0; x < display->bounds.width; x++) {
      const int globalX = display->bounds.x + x;
      const int globalY = display->bounds.y + y;

      pixelBuffer.setPixel(globalX, globalY, displayBuffer.getPixel(x, y));
    }
  }
}

bool LayoutRenderer::setActiveLayout(const char* layoutId) {
  if (layoutId == nullptr) {
    return false;
  }

  const Layout* layout = findLayoutById(board, layoutId);

  if (layout == nullptr) {
    return false;
  }

  currentLayoutId = layout->id;
  return true;
}

const char* LayoutRenderer::activeLayoutId() const {
  return currentLayoutId.c_str();
}

LEDPixel LayoutRenderer::pixelAt(int x, int y) const {
  return pixelBuffer.getPixel(x, y);
}

ValidationResult LayoutRenderer::validate(const LayoutValidator& validator) const {
  return validator.validate(board);
}

const Layout* LayoutRenderer::activeLayout() const {
  return findLayoutById(board, currentLayoutId);
}

DisplayLEDPixelBuffer LayoutRenderer::displayLEDPixelBufferFor(
  const Display& display
) const {
  DisplayLEDPixelBuffer displayBuffer(
    display.bounds.width,
    display.bounds.height
  );

  for (int y = 0; y < display.bounds.height; y++) {
    for (int x = 0; x < display.bounds.width; x++) {
      const bool isBorder =
        x == 0 ||
        y == 0 ||
        x == display.bounds.width - 1 ||
        y == display.bounds.height - 1;

      if (isBorder) {
        displayBuffer.setPixel(x, y, White);
      }
    }
  }

  return displayBuffer;
}
