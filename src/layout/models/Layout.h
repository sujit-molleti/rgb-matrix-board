#pragma once

#include <string>
#include <vector>

#include "Display.h"

struct Layout {
  std::string id;
  std::string name;
  std::vector<Display> displays;
};
