#pragma once

#include <string>
#include <vector>

#include "Layout.h"

struct VirtualRgbBoard {
  int width;
  int height;
  std::string defaultLayoutId;
  std::vector<Layout> layouts;
};
