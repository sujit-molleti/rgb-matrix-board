#pragma once

#include "LayoutRenderer.h"

class BoardCoordinator {
public:
  BoardCoordinator(int boardWidth, int boardHeight);

  void begin(unsigned long nowMs = 0);
  bool tick(unsigned long nowMs);
  const LayoutRenderer& renderer() const;

private:
  LayoutRenderer layoutRenderer;
  unsigned long lastLayoutSwitchMs;
  bool showingHeaderLayout;

  bool switchToLayout(const char* layoutId);
};
