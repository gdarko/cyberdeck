#include "WeatherScreen.h"
#include "config_defaults.h"

WeatherScreen::WeatherScreen(LiquidCrystal_I2C &lcd, HTTPClient &hclient,
                             WiFiClientSecure &wclient, NTPClient &timeClient)
    : Screen(lcd), hclient_(hclient), wclient_(wclient), timeClient_(timeClient) {
  l1_ = CFG_LABEL_WEATHER;
  l2_ = "Loading...";
}

unsigned long WeatherScreen::intervalMs() const {
  return CFG_INTERVAL_WEATHER_MS;
}

void WeatherScreen::refresh() {
  timeClient_.update();
  Serial.println("[APP] weather GET " CFG_URL_WEATHER);
  hclient_.begin(wclient_, CFG_URL_WEATHER);
  // wttr.in returns the HTML browser page for browser-like UAs. A curl-style
  // UA is required to get the plain-text format response.
  hclient_.setUserAgent("curl/8.4.0");
  int code = hclient_.GET();
  if (code != HTTP_CODE_OK) {
    hclient_.end();
    char buf[CFG_LCD_COLS + 1];
    snprintf(buf, sizeof(buf), "HTTP %d", code);
    l1_ = "WEATHER ERR";
    l2_ = buf;
    Serial.print("[APP] weather ");
    Serial.println(l2_);
    return;
  }

  String body = hclient_.getString();
  hclient_.end();
  Serial.print("[APP] weather raw: ");
  Serial.println(body);

  // HD44780 can't render the UTF-8 degree sign (0xC2 0xB0). Strip it.
  body.replace("\xC2\xB0", "");
  body.trim();
  int nl = body.indexOf('\n');
  const char *desc = body.c_str();
  const char *temp = "";
  if (nl >= 0) {
    body.setCharAt(nl, '\0');
    temp = body.c_str() + nl + 1;
  }

  // Bake current local time and short date into the cached temperature line.
  // There is no periodic redraw, so the clock only advances on the next refetch.
  time_t epoch = timeClient_.getEpochTime();
  struct tm *t = gmtime(&epoch);

  char b1[CFG_LCD_COLS + 1];
  char b2[CFG_LCD_COLS + 1];
  snprintf(b1, sizeof(b1), "%s", desc);
  snprintf(b2, sizeof(b2), "%s %02d:%02d %02d/%02d",
           temp, t->tm_hour, t->tm_min,
           t->tm_mday, t->tm_mon + 1);
  l1_ = b1;
  l2_ = b2;

  Serial.print("[APP] weather: ");
  Serial.print(l1_);
  Serial.print(" | ");
  Serial.println(l2_);
}
