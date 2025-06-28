#include "CurrencyScreen.h"
#include <ArduinoJson.h>
#include "config_defaults.h"

namespace {
struct FetchResult {
  String value;
  bool isError = false;
  String error;
};

FetchResult fetchRate(HTTPClient &hclient, WiFiClientSecure &wclient) {
  FetchResult r;
  hclient.begin(wclient, CFG_URL_CURRENCY);
  hclient.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/132.0.0.0 Safari/537.36");
  int code = hclient.GET();

  if (code != HTTP_CODE_OK) {
    r.isError = true;
    r.error   = String("HTTP ") + code;
    hclient.end();
    return r;
  }

  StaticJsonDocument<32> filter;
  filter["rate"] = true;

  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, hclient.getStream(),
                                             DeserializationOption::Filter(filter));
  hclient.end();
  if (err) {
    r.isError = true;
    r.error   = err.f_str();
    Serial.print("[APP] JSON parse error: ");
    Serial.println(err.c_str());
    return r;
  }

  float rate = doc["rate"] | 0.0f;
  r.value = String(rate, 4);
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
  Serial.println("[APP] rate=" + r.value + " at " + formattedTime);

  if (r.isError) {
    l1_ = CFG_LABEL_CURRENCY " err";
    l2_ = r.error;
  } else {
    l1_ = CFG_LABEL_CURRENCY " " + r.value;
    l2_ = "UPDATE " + formattedTime;
  }
}
