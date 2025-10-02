#include "CurrencyScreen.h"
#include <ArduinoJson.h>
#include "config_defaults.h"

namespace {
struct FetchResult {
  char value[12];
  char error[12];
  bool isError = false;
};

FetchResult fetchRate(HTTPClient &hclient, WiFiClientSecure &wclient) {
  FetchResult r;
  r.value[0] = 0;
  r.error[0] = 0;
  hclient.begin(wclient, CFG_URL_CURRENCY);
  hclient.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/132.0.0.0 Safari/537.36");
  int code = hclient.GET();

  if (code != HTTP_CODE_OK) {
    r.isError = true;
    snprintf(r.error, sizeof(r.error), "HTTP %d", code);
    hclient.end();
    return r;
  }

  String body = hclient.getString();
  hclient.end();

  StaticJsonDocument<32> filter;
  filter["rate"] = true;

  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, body,
                                             DeserializationOption::Filter(filter));
  if (err) {
    r.isError = true;
    snprintf(r.error, sizeof(r.error), "%s", err.c_str());
    Serial.print("[APP] JSON parse error: ");
    Serial.println(err.c_str());
    Serial.print("[APP] body: ");
    Serial.println(body);
    return r;
  }

  float rate = doc["rate"] | 0.0f;
  snprintf(r.value, sizeof(r.value), "%.4f", rate);
  return r;
}
} // namespace

CurrencyScreen::CurrencyScreen(LiquidCrystal_I2C &lcd, HTTPClient &hclient,
                               WiFiClientSecure &wclient, NTPClient &timeClient)
    : Screen(lcd), hclient_(hclient), wclient_(wclient), timeClient_(timeClient) {
  l1_ = CFG_LABEL_CURRENCY;
  l2_ = "Loading...";
}

unsigned long CurrencyScreen::intervalMs() const {
  return CFG_INTERVAL_CURRENCY_MS;
}

void CurrencyScreen::refresh() {
  timeClient_.update();
  String formattedTime = timeClient_.getFormattedTime();

  FetchResult r = fetchRate(hclient_, wclient_);
  Serial.print("[APP] rate=");
  Serial.print(r.value);
  Serial.print(" at ");
  Serial.println(formattedTime);

  char b1[CFG_LCD_COLS + 1];
  char b2[CFG_LCD_COLS + 1];
  if (r.isError) {
    snprintf(b1, sizeof(b1), CFG_LABEL_CURRENCY " err");
    snprintf(b2, sizeof(b2), "%s", r.error);
  } else {
    snprintf(b1, sizeof(b1), CFG_LABEL_CURRENCY " %s", r.value);
    snprintf(b2, sizeof(b2), "UPDATE %s", formattedTime.c_str());
  }
  l1_ = b1;
  l2_ = b2;
}
