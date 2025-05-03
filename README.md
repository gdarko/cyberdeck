# Cyberdeck

Mostly a hobby project for learning embedded programming. Small WiFi-connected info screen that lives on my desk. A button on the shield cycles through three screens:

1. **Weather**.  [wttr.in](https://wttr.in), with embedded local clock and short date.
2. **Currency**. Currency rate from Revolut's public quote API.
3. **System health**. Free heap and uptime.

# Hardware

- Wemos D1 Mini (ESP8266)
- 16x2 I2C LCD (HD44780, address `0x3F`)
- LOLIN 1-Button Shield V2.1.0 (right-cross jumper bridged to **D7 / GPIO13**)

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
