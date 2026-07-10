#pragma once

#include "../models/VirtualRgbBoard.h"

class LayoutConfigLoader {
public:
  VirtualRgbBoard load(const char* json) const;
};
