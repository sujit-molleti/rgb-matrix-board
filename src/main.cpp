#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "layout/layout.h"

#define MATRIX_PIN 2
#define ROWS 64
#define COLS 128
#define NUM_PIXELS (ROWS * COLS)

Adafruit_NeoPixel matrix(NUM_PIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

const char* fakeJson = R"json(
{
  "game": {
    "awayTeam": "JETS",
    "awayScore": 24,
    "homeTeam": "PATS",
    "homeScore": 16,
    "quarter": 4,
    "clock": "1:29",
    "possession": "JETS"
  }
}
)json";

uint32_t green = matrix.Color(0, 180, 80);
uint32_t red = matrix.Color(220, 20, 30);
uint32_t white = matrix.Color(220, 220, 220);
uint32_t amber = matrix.Color(255, 150, 0);
uint32_t dimGreen = matrix.Color(0, 60, 30);

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

byte glyphRow(char c, int row) {
  switch (c) {
    case '0': return (byte[]){0b111, 0b101, 0b101, 0b101, 0b111}[row];
    case '1': return (byte[]){0b010, 0b110, 0b010, 0b010, 0b111}[row];
    case '2': return (byte[]){0b111, 0b001, 0b111, 0b100, 0b111}[row];
    case '3': return (byte[]){0b111, 0b001, 0b111, 0b001, 0b111}[row];
    case '4': return (byte[]){0b101, 0b101, 0b111, 0b001, 0b001}[row];
    case '5': return (byte[]){0b111, 0b100, 0b111, 0b001, 0b111}[row];
    case '6': return (byte[]){0b111, 0b100, 0b111, 0b101, 0b111}[row];
    case '7': return (byte[]){0b111, 0b001, 0b010, 0b010, 0b010}[row];
    case '8': return (byte[]){0b111, 0b101, 0b111, 0b101, 0b111}[row];
    case '9': return (byte[]){0b111, 0b101, 0b111, 0b001, 0b111}[row];
    case 'A': return (byte[]){0b010, 0b101, 0b111, 0b101, 0b101}[row];
    case 'B': return (byte[]){0b110, 0b101, 0b110, 0b101, 0b110}[row];
    case 'E': return (byte[]){0b111, 0b100, 0b110, 0b100, 0b111}[row];
    case 'J': return (byte[]){0b001, 0b001, 0b001, 0b101, 0b111}[row];
    case 'L': return (byte[]){0b100, 0b100, 0b100, 0b100, 0b111}[row];
    case 'N': return (byte[]){0b101, 0b111, 0b111, 0b111, 0b101}[row];
    case 'P': return (byte[]){0b110, 0b101, 0b110, 0b100, 0b100}[row];
    case 'Q': return (byte[]){0b111, 0b101, 0b101, 0b111, 0b001}[row];
    case 'S': return (byte[]){0b111, 0b100, 0b111, 0b001, 0b111}[row];
    case 'T': return (byte[]){0b111, 0b010, 0b010, 0b010, 0b010}[row];
    case 'Y': return (byte[]){0b101, 0b101, 0b010, 0b010, 0b010}[row];
    case ':': return (byte[]){0b000, 0b010, 0b000, 0b010, 0b000}[row];
    case '>': return (byte[]){0b100, 0b010, 0b001, 0b010, 0b100}[row];
    case ' ': return 0b000;
    default: return 0b000;
  }
}

void drawChar(int x, int y, char c, uint32_t color) {
  for (int row = 0; row < 5; row++) {
    byte bits = glyphRow(c, row);

    for (int col = 0; col < 3; col++) {
      if (bits & (0b100 >> col)) {
        setPixel(x + col, y + row, color);
      }
    }
  }
}

void drawText(int x, int y, const String& text, uint32_t color) {
  for (int i = 0; i < text.length(); i++) {
    drawChar(x + (i * 4), y, text[i], color);
  }
}

void drawScoreboard(JsonObject game) {
  String awayTeam = game["awayTeam"] | "JETS";
  int awayScore = game["awayScore"] | 24;
  String homeTeam = game["homeTeam"] | "PATS";
  int homeScore = game["homeScore"] | 16;
  int quarter = game["quarter"] | 4;
  String clock = game["clock"] | "1:29";
  String possession = game["possession"] | "";

  matrix.clear();

  for (int x = 0; x < COLS; x++) {
    setPixel(x, 5, dimGreen);
    setPixel(x, 10, matrix.Color(45, 45, 45));
  }

  drawText(0, 0, awayTeam, green);
  drawText(18, 0, String(awayScore), white);

  if (possession == awayTeam) {
    drawText(28, 0, ">", amber);
  }

  drawText(0, 6, homeTeam, red);
  drawText(18, 6, String(homeScore), white);

  if (possession == homeTeam) {
    drawText(28, 6, ">", amber);
  }

  drawText(0, 11, "Q" + String(quarter) + " " + clock, amber);
  drawText(32, 11, "BALL " + possession, green);

  matrix.show();
}

void renderJson(const char* json) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  drawScoreboard(doc["game"].as<JsonObject>());
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
  LayoutValidator validator(COLS, ROWS);
  ValidationResult validation = layoutRenderer.validate(validator);
  printValidationResult(validation);

  Serial.println("Rendering fake scoreboard JSON...");
  renderJson(fakeJson);
}

void loop() {
  delay(1000);
}
