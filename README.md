# RGB Sports Matrix

RGB Sports Matrix is an ESP32-powered C++ project for rendering compact sports information on an RGB LED matrix. Using PlatformIO, Arduino, Adafruit NeoPixel, and ArduinoJson to display information.

The longer-term goal is to build a modular rendering pipeline capable of displaying live sports content from multiple leagues and providers while staying independent from any single sports API.

## Current Status

This repository is an early prototype. It currently supports:

- ESP32 DevKit target through PlatformIO.
- 64 row x 128 column Wokwi LED matrix simulation.
- Serpentine pixel mapping.
- JSON parsing with ArduinoJson.
- A hardcoded sample football scoreboard.

The project is being shaped toward a general sports display system that can render live events, upcoming games, completed results, secondary score tiles, standings, news alerts, and ticker content.

## Hardware Model

**To add more information here as software starts to finalize on simulator...**

The checked-in Wokwi circuit currently uses:

- ESP32 DevKit V1.
- 64x128 RGB LED matrix.
- Matrix data pin connected to ESP32 `D2`.
- Serpentine matrix layout.

The firmware currently defines:

```cpp
#define MATRIX_PIN 2
#define ROWS 64
#define COLS 128
```

The target architecture is being designed around the 128x64 display layout now reflected in the firmware and Wokwi diagram. The goals is to take 4 64 x 32(W x L) and create a 2 x 2 grid, creating a large 128 x 64 panel.

## Project Structure

```text
.
├── diagram.json      # Wokwi ESP32 + LED matrix circuit
├── platformio.ini   # PlatformIO environment and library dependencies
├── src/main.cpp     # Current rendering prototype
└── wokwi.toml       # Wokwi project configuration
```

## Dependencies

PlatformIO installs the project dependencies declared in `platformio.ini`:

- `adafruit/Adafruit NeoPixel`
- `bblanchon/ArduinoJson`

## Running The Project

Build the firmware:

```sh
pio run
```

Upload to a connected ESP32:

```sh
pio run --target upload
```

Open the serial monitor:

```sh
pio device monitor
```

The project can also be run in Wokwi using the included `diagram.json` and `wokwi.toml` files.

## High-Level Architecture

```text
Sports APIs
        |
        v
     Adapters
        |
        v
   Content Models
        |
        v
 Content ViewModels
        |
        v
   Content Views
        |
        v
    Coordinator
        |
        v
   Display Zones
        |
        v
 RGB Matrix Renderer
```

Each layer owns a single responsibility.

The architecture emphasizes:

- Separation of concerns.
- Extensibility.
- Testability.
- Clear domain modeling.
- API-independent rendering.

## Design Principles

### Normalize External APIs

Every sports provider exposes different schemas. Adapters are responsible only for converting external API responses into internal domain models.

Adapters should not make display, layout, or prioritization decisions.

### Separate Data From Presentation

The architecture intentionally separates sports data from matrix presentation:

```text
ContentModel
      |
      v
ContentViewModel
      |
      v
ContentView
      |
      v
Renderer
```

The domain model represents what the data is. The rendering layer determines how that data is displayed.

### Prefer Composition For Domain Models

The sports data layer should use composition instead of a data-only inheritance hierarchy. Shared fields should live in metadata objects owned by concrete domain models.

Example direction:

```cpp
struct ContentMetadata {
    std::string id;
    Provider provider;
    std::string providerId;

    Sport sport;
    std::optional<League> league;

    std::chrono::system_clock::time_point fetchedAt;
};

struct Event {
    ContentMetadata metadata;

    std::string title;
    EventStatus status;
    std::vector<Competitor> competitors;

    std::optional<std::string> location;
    std::chrono::system_clock::time_point scheduledAt;
};
```

### Display Zones Are Layout Containers

Display Zones define where content is rendered. They do not define how content should look, and they should remain unaware of the content they contain.

Current target V1 layout:

```text
+------------------------------------------------------------------+
|                                                                  |
|                            PRIMARY                               |
|                                                                  |
+---------------+---------------+---------------+------------------+
| SECONDARY 1   | SECONDARY 2   | SECONDARY 3   | SECONDARY 4      |
+---------------+---------------+---------------+------------------+
|                      SCROLLING TICKER                            |
+------------------------------------------------------------------+
```

Approximate target dimensions:

```text
Primary          128 x 40

Secondary 1       32 x 16
Secondary 2       32 x 16
Secondary 3       32 x 16
Secondary 4       32 x 16

Ticker           128 x 8
```

Display Zones simply provide rectangular bounds to Content Views.

## Domain Model

The root content model direction is:

```text
ContentModel
├── Event
├── Standing
└── NewsAlert
```

The domain model represents information that exists within the sports domain. It should remain independent from rendering.

## Event

An Event represents a scheduled, live, or completed sports competition.

Examples:

- NFL game.
- NBA game.
- MLB game.
- Tennis match.
- Golf tournament.
- Formula 1 race.

Current planned fields:

- `id`
- `provider`
- `providerId`
- `sport`
- `league`
- `state`
- `title`
- `competitors`
- `location`
- `scheduledAt`
- `fetchedAt`
- `statusText`

## Competitor

A Competitor represents an entity participating in an Event.

Current planned fields:

- `id`
- `type`
- `displayName`
- `abbreviation`
- `country`
- `affiliation`
- `stats`
- `score`

Examples:

```text
NFL
displayName = New York Jets
record = 10-7
score = 24

Tennis
displayName = Carlos Alcaraz
country = ESP
rank = 2
score = 6-4 3-2

Golf
displayName = Scottie Scheffler
country = USA
rank = 1
score = -8

Formula 1
displayName = Max Verstappen
country = NED
affiliation = Red Bull
score = P1
```

## Standing

A Standing represents standings, rankings, or tables.

Examples:

- AFC East standings.
- NBA Eastern Conference.
- ATP rankings.
- Formula 1 Constructors Championship.

## NewsAlert

A NewsAlert represents informational sports content.

Examples:

- Trade news.
- Injury reports.
- Breaking news.
- Roster moves.

## Rendering Pipeline

The rendering pipeline follows:

```text
Raw API Data
        |
        v
Adapter
        |
        v
ContentModel
        |
        v
ContentViewModel
        |
        v
ContentView
        |
        v
Coordinator
        |
        v
DisplayZone
        |
        v
Renderer
```

The display layer should receive compact view models rather than raw provider JSON or full sports domain objects.

This keeps provider-specific parsing, sports-specific modeling, content prioritization, and matrix rendering isolated from each other.

## Content Views

Each ContentModel is transformed into a corresponding ViewModel and then into a ContentView.

Examples:

```text
Event
    |
    v
EventViewModel
    |
    v
EventView

Standing
    |
    v
StandingViewModel
    |
    v
StandingView

NewsAlert
    |
    v
NewsViewModel
    |
    v
NewsView
```

Each ContentView is responsible for rendering itself inside the bounds supplied by a DisplayZone. The DisplayZone should never know what type of content it contains.

## Current Scope

The initial implementation order is:

1. Domain Models
2. Display Zone definitions
3. Unit Tests
4. API Adapters
5. ViewModels
6. Content Views
7. RGB Matrix Renderer
8. Content Prioritization / Ranking

The ranking system has intentionally been deferred until the rendering architecture has been established.

## Future Prioritization

The Coordinator will eventually be responsible for assigning content to Display Zones.

Current thinking:

1. Collect all available content.
2. Normalize provider responses into ContentModels.
3. Build ContentViewModels.
4. Build ContentViews.
5. Assign the most appropriate content to Primary, Secondary 1-4, and Ticker zones.

The ranking algorithm will be implemented after the rendering system is complete.

## Roadmap

- Move the current rendering code into focused display primitives.
- Define sports domain models for events, competitors, scores, status, providers, sports, and leagues.
- Add display zone definitions for primary, secondary, and ticker regions.
- Add matrix-specific view models for primary, secondary, and ticker regions.
- Add provider mappers that convert API JSON into normalized domain models.
- Render multiple sports formats, starting with football-style team matchups.
- Support live, scheduled, completed, delayed, postponed, and cancelled event states.
- Add content prioritization once the rendering architecture is stable.

## Goal

The architecture should make adding new sports or content types straightforward.

Adding a new content type should ideally require:

1. Creating a new ContentModel.
2. Writing an Adapter.
3. Creating a ViewModel.
4. Implementing a ContentView.

No changes should be required to the Display Zones, Coordinator interface, or rendering pipeline.
