#pragma once
#include "../Screen.h"

class SystemHealthScreen : public Screen {
public:
  SystemHealthScreen(LiquidCrystal_I2C &lcd) : Screen(lcd) {
    l1_ = "HEAP";
    l2_ = "UP";
  }

  const char *name() const override { return "health"; }

protected:
  void refresh() override;
  unsigned long intervalMs() const override;
  bool refreshOnImmediate() const override { return true; }
};
