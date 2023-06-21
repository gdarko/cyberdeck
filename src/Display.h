#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

void Display_LCD_Setup(LiquidCrystal_I2C &lcd);

// Writes both lines without lcd.clear(). Each line is padded or truncated to
// exactly CFG_LCD_COLS chars so leftover characters from a previous frame
// are overwritten. This avoids the visible flicker lcd.clear() causes.
void Display_LCD_Message(LiquidCrystal_I2C &lcd, const String &line1, const String &line2);
