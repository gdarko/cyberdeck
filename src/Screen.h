#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Display.h"
#include "config_defaults.h"

// Abstract base for an LCD screen. Subclasses implement refresh() (which
// updates l1_ and l2_) and intervalMs(). The base's tick() runs the gating
// algorithm: refresh on first run or once the interval has elapsed, redraw
// the cached strings on demand (button press) without paying for a refetch.
class Screen {
public:
  virtual ~Screen() = default;
  virtual const char *name() const = 0;

  void tick(bool immediate) {
    const unsigned long now = millis();
    const bool firstRun    = (lastRefresh_ == 0);
    const bool intervalDue = (now - lastRefresh_) >= intervalMs();
    const bool needRefresh = firstRun || intervalDue
                          || (immediate && refreshOnImmediate());
    const bool draw        = immediate || needRefresh;
    if (needRefresh) {
      refresh();
      lastRefresh_ = millis();
    }
    if (draw && (immediate || l1_ != prevL1_ || l2_ != prevL2_)) {
      Display_LCD_Message(lcd_, l1_, l2_);
      prevL1_ = l1_;
      prevL2_ = l2_;
    }
  }

protected:
  Screen(LiquidCrystal_I2C &lcd) : lcd_(lcd) {
    l1_.reserve(CFG_LCD_COLS + 1);
    l2_.reserve(CFG_LCD_COLS + 1);
    prevL1_.reserve(CFG_LCD_COLS + 1);
    prevL2_.reserve(CFG_LCD_COLS + 1);
  }

  virtual void refresh() = 0;
  virtual unsigned long intervalMs() const = 0;

  // Return true to recompute on a button-press redraw rather than just
  // re-blitting the cached strings. Useful for cheap, no-network screens
  // where showing stale state on demand is undesirable.
  virtual bool refreshOnImmediate() const { return false; }

  LiquidCrystal_I2C &lcd_;
  unsigned long lastRefresh_ = 0;
  String l1_;
  String l2_;
  String prevL1_;
  String prevL2_;
};
