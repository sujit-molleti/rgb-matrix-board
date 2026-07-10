#pragma once

#include <string>

#include "Rect.h"

enum class DisplayRole {
  Primary,
  Secondary,
  Header,
  Ticker,
  Unknown
};

struct Display {
  std::string id;
  DisplayRole role;
  Rect bounds;
};
