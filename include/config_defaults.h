#pragma once

// Cyberdeck compile-time configuration.
// Optional include/config.h (gitignored) overrides any default below.
// Copy include/config.example.h to include/config.h to start.

#if __has_include("config.h")
  #include "config.h"
#endif

#define CFG_STR_(x) #x
#define CFG_STR(x)  CFG_STR_(x)

#ifndef CFG_VERSION
  #define CFG_VERSION              "1.0"
#endif
#ifndef CFG_WIFI_AP_NAME
  #define CFG_WIFI_AP_NAME         "Cyberdeck Setup"
#endif
#ifndef CFG_WIFI_AP_PASSPHRASE
  #define CFG_WIFI_AP_PASSPHRASE   "123123123"
#endif
// GPIO13. LOLIN 1-Button Shield V2.1.0: bridge the right-cross center pad to
// the `D7 / 13` pad. D7 has no boot-mode constraint, unlike D3/GPIO0, so the
// button is safe to hold across a reset.
#ifndef CFG_BUTTON_PIN
  #define CFG_BUTTON_PIN           D7
#endif
#ifndef CFG_WEATHER_LOCATION
  #define CFG_WEATHER_LOCATION     Kichevo
#endif
#ifndef CFG_CURRENCY_BASE
  #define CFG_CURRENCY_BASE        USD
#endif
#ifndef CFG_CURRENCY_QUOTE
  #define CFG_CURRENCY_QUOTE       EUR
#endif
#ifndef CFG_INTERVAL_CURRENCY_MS
  #define CFG_INTERVAL_CURRENCY_MS 120000UL  // 2 min
#endif
#ifndef CFG_INTERVAL_WEATHER_MS
  #define CFG_INTERVAL_WEATHER_MS  600000UL  // 10 min
#endif
#ifndef CFG_INTERVAL_HEALTH_MS
  #define CFG_INTERVAL_HEALTH_MS     1000UL  // 1 sec (uptime ticks)
#endif
#ifndef CFG_TIMEZONE_OFFSET
  #define CFG_TIMEZONE_OFFSET      7200
#endif
#ifndef CFG_NTP_SERVER
  #define CFG_NTP_SERVER           "pool.ntp.org"
#endif
#ifndef CFG_LCD_I2C_ADDR
  #define CFG_LCD_I2C_ADDR         0x3F
#endif
#ifndef CFG_LCD_COLS
  #define CFG_LCD_COLS             16
#endif
#ifndef CFG_LCD_ROWS
  #define CFG_LCD_ROWS             2
#endif

// Derived from the parts above. Do not override directly.
#define CFG_URL_WEATHER    "https://wttr.in/" CFG_STR(CFG_WEATHER_LOCATION) "?format=%C%0A%t"
#define CFG_URL_CURRENCY   "https://www.revolut.com/api/quote/public/" \
                           CFG_STR(CFG_CURRENCY_BASE) CFG_STR(CFG_CURRENCY_QUOTE)
#define CFG_LABEL_WEATHER  CFG_STR(CFG_WEATHER_LOCATION)
#define CFG_LABEL_CURRENCY CFG_STR(CFG_CURRENCY_BASE) "/" CFG_STR(CFG_CURRENCY_QUOTE)
