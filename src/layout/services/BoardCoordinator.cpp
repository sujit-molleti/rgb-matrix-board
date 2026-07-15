#include "BoardCoordinator.h"

namespace {
const char* const GameDayLayoutId = "game_day";
const char* const GameDayHeaderLayoutId = "game_day_header";
const unsigned long LayoutSwitchIntervalMs = 5000;
}

BoardCoordinator::BoardCoordinator(int boardWidth, int boardHeight)
  : layoutRenderer(boardWidth, boardHeight),
    lastLayoutSwitchMs(0),
    showingHeaderLayout(false) {
}

void BoardCoordinator::begin(unsigned long nowMs) {
  showingHeaderLayout = false;
  lastLayoutSwitchMs = nowMs;
  switchToLayout(GameDayLayoutId);
}

bool BoardCoordinator::tick(unsigned long nowMs) {
  if (nowMs - lastLayoutSwitchMs < LayoutSwitchIntervalMs) {
    return false;
  }

  const char* nextLayoutId = showingHeaderLayout
    ? GameDayLayoutId
    : GameDayHeaderLayoutId;

  if (!switchToLayout(nextLayoutId)) {
    return false;
  }

  showingHeaderLayout = !showingHeaderLayout;
  lastLayoutSwitchMs = nowMs;
  return true;
}

const LayoutRenderer& BoardCoordinator::renderer() const {
  return layoutRenderer;
}

bool BoardCoordinator::switchToLayout(const char* layoutId) {
  if (!layoutRenderer.setActiveLayout(layoutId)) {
    return false;
  }

  layoutRenderer.clear();
  layoutRenderer.renderDisplayBorders();
  return true;
}
