#ifndef PIO_UNIT_TESTING

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "layout/config/settings.h"
#include "layout/layout.h"

#define MATRIX_PIN 2
#define ROWS 64
#define COLS 128
#define NUM_PIXELS (ROWS * COLS)

Adafruit_NeoPixel matrix(NUM_PIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

int pixelIndex(int x, int y) {
  if (y % 2 == 0) {
    return y * COLS + x;
  }

  return y * COLS + (COLS - 1 - x);
}

void setPixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
    return;
  }

  matrix.setPixelColor(pixelIndex(x, y), color);
}

uint32_t ledPixelColor(const LEDPixel& pixel) {
  return matrix.Color(pixel.red, pixel.green, pixel.blue);
}

void renderLayoutRenderer(LayoutRenderer& layoutRenderer) {
  matrix.clear();

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      setPixel(x, y, ledPixelColor(layoutRenderer.pixelAt(x, y)));
    }
  }

  matrix.show();
}

void printValidationResult(const ValidationResult& result) {
  if (result.isValid) {
    Serial.println("Layout validation passed.");
    return;
  }

  Serial.println("Layout validation failed:");

  for (const ValidationError& error : result.errors) {
    Serial.print("- ");
    Serial.print(validationErrorTypeName(error.type));
    Serial.print(": ");
    Serial.print(error.message.c_str());

    if (!error.layoutId.empty()) {
      Serial.print(" layout=");
      Serial.print(error.layoutId.c_str());
    }

    if (!error.displayId.empty()) {
      Serial.print(" display=");
      Serial.print(error.displayId.c_str());
    }

    if (error.x >= 0 && error.y >= 0) {
      Serial.print(" x=");
      Serial.print(error.x);
      Serial.print(" y=");
      Serial.print(error.y);
    }

    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(200);

  LayoutRenderer layoutRenderer(COLS, ROWS);

  if (!layoutRenderer.setActiveLayout(layout_settings::ActiveLayoutId)) {
    Serial.println(
      "Failed to activate " + String(layout_settings::ActiveLayoutId) + " layout."
    );
  }

  LayoutValidator validator(COLS, ROWS);
  ValidationResult validation = layoutRenderer.validate(validator);
  printValidationResult(validation);

  Serial.println("Rendering layout display borders...");
  layoutRenderer.renderDisplayBorders();
  renderLayoutRenderer(layoutRenderer);
}

void loop() {
  delay(1000);
}

#endif
