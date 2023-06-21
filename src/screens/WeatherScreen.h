#pragma once
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include "../Screen.h"

class WeatherScreen : public Screen {
public:
  WeatherScreen(LiquidCrystal_I2C &lcd, HTTPClient &hclient,
                WiFiClientSecure &wclient, NTPClient &timeClient);

  const char *name() const override { return "weather"; }

protected:
  void refresh() override;
  unsigned long intervalMs() const override;

private:
  HTTPClient &hclient_;
  WiFiClientSecure &wclient_;
  NTPClient &timeClient_;
};
