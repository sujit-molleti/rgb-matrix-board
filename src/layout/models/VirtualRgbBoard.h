#pragma once

#include <string>
#include <vector>

#include "Layout.h"

struct VirtualRgbBoard {
  int width = 0;
  int height = 0;
  std::string defaultLayoutId;
  std::vector<Layout> layouts;
};
