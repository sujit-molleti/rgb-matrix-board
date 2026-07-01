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

LEDPixel LayoutRenderer::pixelAt(int x, int y) const {
  return pixelBuffer.getPixel(x, y);
}

ValidationResult LayoutRenderer::validate(const LayoutValidator& validator) const {
  VirtualRgbBoard board = createDefaultBoard(boardWidth, boardHeight);
  return validator.validate(board);
}
