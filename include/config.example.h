#pragma once

// Cyberdeck personal config. Copy this file to `config.h` (same dir) and edit.
// `config.h` is gitignored. Anything you don't override falls back to the
// defaults baked into include/config_defaults.h.

// ============================================================================
// MUST CHANGE: these are personal to your setup
// ============================================================================

// City for wttr.in. Bare token, NOT a string. Single word; for multi-word
// cities use the wttr.in URL form (e.g. `New_York`).
#define CFG_WEATHER_LOCATION  Kichevo

// Currency pair fetched from Revolut's public quote API. Bare 3-letter tokens.
// Combined into the URL as `<BASE><QUOTE>` (e.g. `USDEUR`) and into the LCD
// label as `<BASE>/<QUOTE>` (e.g. `USD/EUR`).
#define CFG_CURRENCY_BASE     USD
#define CFG_CURRENCY_QUOTE    EUR

// Local timezone offset from UTC, in seconds. No DST handling; adjust manually.
//   UTC+2 -> 7200,  UTC-5 -> -18000,  UTC+5:30 -> 19800
#define CFG_TIMEZONE_OFFSET   7200

// First-boot WiFi setup portal. Change the passphrase before deploying.
#define CFG_WIFI_AP_PASSPHRASE "123123123"

// ============================================================================
// Rarely changed: only touch these if your hardware differs
// ============================================================================

// #define CFG_WIFI_AP_NAME       "Cyberdeck Setup"
// #define CFG_NTP_SERVER         "pool.ntp.org"

// HD44780 LCD. I2C address is usually 0x27 or 0x3F. Run an i2c scanner if unsure.
// #define CFG_LCD_I2C_ADDR       0x3F
// #define CFG_LCD_COLS           16
// #define CFG_LCD_ROWS           2

// Button shield GPIO. Bare token (D0..D8). Avoid D3/GPIO0 (boot-mode pin).
// #define CFG_BUTTON_PIN         D7

// Refresh intervals (ms).
// #define CFG_INTERVAL_CURRENCY_MS  120000UL  // 2 min
// #define CFG_INTERVAL_WEATHER_MS   600000UL  // 10 min
// #define CFG_INTERVAL_HEALTH_MS      1000UL  // 1 sec
