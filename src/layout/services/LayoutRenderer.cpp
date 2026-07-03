#include "LayoutRenderer.h"

#include "LayoutValidator.h"
#include "../internal/layout_private.h"

namespace {
const char* DefaultLayoutId = "game_day";

VirtualRgbBoard createDefaultBoard(int boardWidth, int boardHeight) {
  return {
    boardWidth,
    boardHeight,
    DefaultLayoutId,
    {
      {
        DefaultLayoutId,
        "Game Day",
        {
          {"primary", DisplayRole::Primary, {0, 0, 128, 40}},
          {"secondary_1", DisplayRole::Secondary, {0, 40, 32, 16}},
          {"secondary_2", DisplayRole::Secondary, {32, 40, 32, 16}},
          {"secondary_3", DisplayRole::Secondary, {64, 40, 32, 16}},
          {"secondary_4", DisplayRole::Secondary, {96, 40, 32, 16}},
          {"ticker", DisplayRole::Ticker, {0, 56, 128, 8}}
        }
      }
    }
  };
}

const Layout* findDefaultLayout(const VirtualRgbBoard& board) {
  for (const Layout& layout : board.layouts) {
    if (layout.id == board.defaultLayoutId) {
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
  : boardWidth(boardWidth),
    boardHeight(boardHeight),
    pixelBuffer(boardWidth, boardHeight) {
}

void LayoutRenderer::clear(LEDPixel pixel) {
  pixelBuffer.clear(pixel);
}

void LayoutRenderer::setPixel(int x, int y, LEDPixel pixel) {
  pixelBuffer.setPixel(x, y, pixel);
}

void LayoutRenderer::renderDisplay(
  const char* displayId,
  const DisplayLEDPixelBuffer& displayBuffer
) {
  VirtualRgbBoard board = createDefaultBoard(boardWidth, boardHeight);
  const Layout* layout = findDefaultLayout(board);

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

LEDPixel LayoutRenderer::pixelAt(int x, int y) const {
  return pixelBuffer.getPixel(x, y);
}

ValidationResult LayoutRenderer::validate(const LayoutValidator& validator) const {
  VirtualRgbBoard board = createDefaultBoard(boardWidth, boardHeight);
  return validator.validate(board);
}
