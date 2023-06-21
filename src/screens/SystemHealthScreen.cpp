#include "SystemHealthScreen.h"
#include "config_defaults.h"

unsigned long SystemHealthScreen::intervalMs() const {
  return CFG_INTERVAL_HEALTH_MS;
}

void SystemHealthScreen::refresh() {
  unsigned long s = millis() / 1000;
  unsigned int  d = s / 86400;
  unsigned int  h = (s / 3600) % 24;
  unsigned int  m = (s / 60) % 60;

  char b1[17], b2[17];
  snprintf(b1, sizeof(b1), "HEAP %u B", ESP.getFreeHeap());
  snprintf(b2, sizeof(b2), "UP %ud %02uh %02um", d, h, m);
  l1_ = b1;
  l2_ = b2;
}
