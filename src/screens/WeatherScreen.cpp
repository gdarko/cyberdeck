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
  hclient_.begin(wclient_, CFG_URL_WEATHER);
  // wttr.in returns the HTML browser page for browser-like UAs. A curl-style
  // UA is required to get the plain-text format response.
  hclient_.setUserAgent("curl/8.4.0");
  int code = hclient_.GET();
  if (code != HTTP_CODE_OK) {
    hclient_.end();
    l1_ = "WEATHER ERR";
    l2_ = String("HTTP ") + code;
    Serial.println("[APP] weather " + l2_);
    return;
  }

  String body = hclient_.getString();
  hclient_.end();

  // HD44780 can't render the UTF-8 degree sign (0xC2 0xB0). Strip it.
  body.replace("\xC2\xB0", "");
  body.trim();
  int nl = body.indexOf('\n');
  if (nl >= 0) {
    l1_ = body.substring(0, nl);
    l2_ = body.substring(nl + 1);
  } else {
    l1_ = body;
    l2_ = "";
  }

  // Bake current local time and short date into the cached temperature line.
  // There is no periodic redraw, so the clock only advances on the next refetch.
  time_t epoch = timeClient_.getEpochTime();
  struct tm *t = gmtime(&epoch);
  char suffix[13];
  snprintf(suffix, sizeof(suffix), " %02d:%02d %02d/%02d",
           timeClient_.getHours(), timeClient_.getMinutes(),
           t->tm_mday, t->tm_mon + 1);
  l2_ += suffix;

  Serial.println("[APP] weather: " + l1_ + " | " + l2_);
}
