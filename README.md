# Cyberdeck

Mostly a hobby project for learning embedded programming. Small WiFi-connected info screen that lives on my desk. A button on the shield cycles through three screens:

1. **Weather**.  [wttr.in](https://wttr.in), with embedded local clock and short date.
2. **Currency**. Currency rate from Revolut's public quote API.
3. **System health**. Free heap and uptime.

# Hardware

- Wemos D1 Mini (ESP8266)
- 16x2 I2C LCD (HD44780, address `0x3F`)
- LOLIN 1-Button Shield V2.1.0 (right-cross jumper bridged to **D7 / GPIO13**)

# Implementation

### Architecture

`Screen` (`src/Screen.h`) is an abstract base that owns the tick/refresh/redraw loop. Subclasses implement `refresh()` (which fills the two cached line strings `l1_` and `l2_`) and `intervalMs()`. The base's `tick()` only refetches when the interval has elapsed, and only writes to the LCD when a line actually changed (or unconditionally on a button-press redraw). `main.cpp` holds the screen array and runs a 50 ms loop; the active screen's `tick()` is a no-op when nothing is due.

### Display

`Display_LCD_Message` (`src/Display.cpp`) pads or truncates each line to exactly `CFG_LCD_COLS` characters and rewrites both rows without `lcd.clear()`, so leftover characters from the previous frame are overwritten in place and there's no visible flicker. Combined with the change-detection in `Screen::tick`, idle ticks send zero I2C traffic.

### Networking

- HTTPS via `WiFiClientSecure` with `setInsecure()` and reduced BearSSL buffers (`2048 / 512`) to fit ESP8266 RAM.
- `HTTPClient::setReuse(true)` keeps the TLS session alive across refreshes so subsequent fetches skip the handshake.
- WiFi provisioning via `WiFiManager` (captive portal `Cyberdeck Setup`).
- Time via `NTPClient` with `CFG_TIMEZONE_OFFSET`; the weather screen reuses the cached `gmtime` fields when composing its line.

### Screens

- **Weather** (10 min): `wttr.in` returns the HTML browser page for browser-like UAs, so the request sends `curl/8.4.0` to get the plain-text format. The UTF-8 degree sign (`0xC2 0xB0`) is stripped because HD44780 can't render it.
- **Currency** (2 min): stream-parses Revolut's JSON with an `ArduinoJson` `Filter` that keeps only the `rate` field, so the whole body never needs to fit in the 128-byte `StaticJsonDocument`.
- **System health** (1 s): free heap + uptime, no network.

### Button

Edge-detect HIGH→LOW with 50 ms debounce in `main.cpp`. A press advances `currentScreen` and calls `tick(immediate=true)` to redraw without waiting for the interval.

### Config overrides

`include/config_defaults.h` defines every `CFG_*` symbol behind `#ifndef`, and `__has_include("config.h")` pulls in the user's overrides if present, so customizing one value doesn't require touching the rest.

# Configuration

Personal settings (weather location, currency pair, timezone, AP passphrase) live in `include/config.h`, which is gitignored. To customize:

```
cp include/config.example.h include/config.h
$EDITOR include/config.h        # change the values marked MUST CHANGE
```

The project compiles without `include/config.h`. Defaults are baked into the source, but the AP passphrase will be the public default, so change it before deploying.

# Build / flash

PlatformIO (recommended):

```
pio run                    # build
pio run -t upload          # flash to /dev/ttyUSB0
pio device monitor         # serial monitor at 115200
```

First boot brings up a captive WiFi portal named `Cyberdeck Setup` (passphrase `123123123`). Join it from your phone to enter real WiFi credentials.

# TODO

- Add circuit sketch
- Move to a bigger display (2.4" TFT or 2.42" OLED)

# License

Released into the public domain under [The Unlicense](https://unlicense.org). Do whatever you want with it. No warranty.
