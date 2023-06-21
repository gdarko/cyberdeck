#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiClientSecure.h>

#include "config_defaults.h"
#include "Display.h"
#include "Screen.h"
#include "screens/WeatherScreen.h"
#include "screens/CurrencyScreen.h"
#include "screens/SystemHealthScreen.h"

// Globals. Construction order in this TU is top-to-bottom, so the Screen
// subclasses declared below see these objects fully constructed by the time
// their own constructors run.
LiquidCrystal_I2C lcd(CFG_LCD_I2C_ADDR, CFG_LCD_COLS, CFG_LCD_ROWS);
HTTPClient httpClient;
WiFiClientSecure wifiClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, CFG_NTP_SERVER);

WeatherScreen      weatherScreen(lcd, httpClient, wifiClient, timeClient);
CurrencyScreen     currencyScreen(lcd, httpClient, wifiClient, timeClient);
SystemHealthScreen healthScreen(lcd);

Screen *const screens[] = { &weatherScreen, &currencyScreen, &healthScreen };
constexpr size_t SCREEN_COUNT = sizeof(screens) / sizeof(screens[0]);

unsigned int currentScreen = 0;

void WiFi_Setup();
bool WiFi_Connected();
void NTP_Setup(NTPClient &timeClient);

void setup(){
  Serial.begin(115200);

  pinMode(CFG_BUTTON_PIN, INPUT_PULLUP);

  Display_LCD_Setup(lcd);
  Display_LCD_Message(lcd, "Welcome To", "Cyberdeck v" CFG_VERSION);

  delay(2000);

  WiFi_Setup();
  wifiClient.setInsecure();
  wifiClient.setBufferSizes(2048, 512);
  NTP_Setup(timeClient);
}

void loop(){

  if(!WiFi_Connected()) {
    Serial.println("[WiFi] Not connected (loop).");
    delay(500);
    return;
  }

  // Button: edge-detect HIGH to LOW with 50ms debounce. On click, advance to
  // the next screen and force an immediate redraw of its cached content.
  static int lastBtnState = HIGH;
  static unsigned long lastBtnEdge = 0;
  int btn = digitalRead(CFG_BUTTON_PIN);
  if (lastBtnState == HIGH && btn == LOW && millis() - lastBtnEdge > 50) {
    lastBtnEdge = millis();
    currentScreen = (currentScreen + 1) % SCREEN_COUNT;
    Serial.print("[BTN] -> ");
    Serial.println(screens[currentScreen]->name());
    screens[currentScreen]->tick(/*immediate=*/true);
  }
  lastBtnState = btn;

  // Periodic refresh for the active screen. Each screen owns its own gating
  // (interval plus cached strings); this is a no-op when nothing is due.
  screens[currentScreen]->tick(/*immediate=*/false);
  delay(50);
}

void NTP_Setup(NTPClient &timeClient) {
  timeClient.begin();
  timeClient.setTimeOffset(CFG_TIMEZONE_OFFSET);
  timeClient.forceUpdate();
}

void WiFi_Setup() {
  WiFi.mode(WIFI_STA); // ESP defaults to STA+AP; pin it to STA only.
  WiFiManager wm;

  bool res = wm.autoConnect(CFG_WIFI_AP_NAME, CFG_WIFI_AP_PASSPHRASE);

  if(!res) {
    Serial.println("[WiFi] Failed to connect");
    ESP.restart();
  } else {
    Serial.println("[WiFi] Connected.");
    Serial.println("[WiFi] SSID:    " + WiFi.SSID());
    Serial.println("[WiFi] Channel: " + String(WiFi.channel()));
    Serial.println("[WiFi] RSSI:    " + String(WiFi.RSSI()) + " dBm");
    Serial.println("[WiFi] IP:      " + WiFi.localIP().toString());
  }
}

bool WiFi_Connected() {
  return WiFi.status() == WL_CONNECTED;
}
