#include "Display.h"
#include "config_defaults.h"

void Display_LCD_Setup(LiquidCrystal_I2C &lcd) {
  lcd.begin(CFG_LCD_COLS, CFG_LCD_ROWS);
  lcd.init();
  lcd.backlight();
}

void Display_LCD_Message(LiquidCrystal_I2C &lcd, const String &line1, const String &line2) {
  char buf[CFG_LCD_COLS + 1];
  const char *fmt = "%-" CFG_STR(CFG_LCD_COLS) "." CFG_STR(CFG_LCD_COLS) "s";

  snprintf(buf, sizeof(buf), fmt, line1.c_str());
  lcd.setCursor(0, 0);
  lcd.print(buf);

  snprintf(buf, sizeof(buf), fmt, line2.c_str());
  lcd.setCursor(0, 1);
  lcd.print(buf);
}
