#pragma once
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include "../Screen.h"

class CurrencyScreen : public Screen {
public:
  CurrencyScreen(LiquidCrystal_I2C &lcd, HTTPClient &hclient,
                 WiFiClientSecure &wclient, NTPClient &timeClient);

  const char *name() const override { return "currency"; }

protected:
  void refresh() override;
  unsigned long intervalMs() const override;

private:
  HTTPClient &hclient_;
  WiFiClientSecure &wclient_;
  NTPClient &timeClient_;
};
