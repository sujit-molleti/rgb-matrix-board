#include "LayoutConfigLoader.h"

#include <ArduinoJson.h>

namespace {
DisplayRole displayRoleFromString(const char* role) {
  if (role == nullptr) {
    return DisplayRole::Unknown;
  }

  const std::string roleString(role);

  if (roleString == "primary") {
    return DisplayRole::Primary;
  }

  if (roleString == "secondary") {
    return DisplayRole::Secondary;
  }

  if (roleString == "ticker") {
    return DisplayRole::Ticker;
  }

  return DisplayRole::Unknown;
}
}

VirtualRgbBoard LayoutConfigLoader::load(const char* json) const {
  VirtualRgbBoard board;
  JsonDocument document;
  DeserializationError error = deserializeJson(document, json);

  if (error) {
    return board;
  }

  board.width = document["width"] | 0;
  board.height = document["height"] | 0;
  board.defaultLayoutId = (document["defaultLayoutId"] | "");

  JsonArray layouts = document["layouts"].as<JsonArray>();

  for (JsonObject layoutObject : layouts) {
    Layout layout;
    layout.id = (layoutObject["id"] | "");
    layout.name = (layoutObject["name"] | "");

    JsonArray displays = layoutObject["displays"].as<JsonArray>();

    for (JsonObject displayObject : displays) {
      JsonObject boundsObject = displayObject["bounds"].as<JsonObject>();
      Display display = {
        (displayObject["id"] | ""),
        displayRoleFromString(displayObject["role"] | nullptr),
        {
          boundsObject["x"] | 0,
          boundsObject["y"] | 0,
          boundsObject["width"] | 0,
          boundsObject["height"] | 0
        }
      };

      layout.displays.push_back(display);
    }

    board.layouts.push_back(layout);
  }

  return board;
}
