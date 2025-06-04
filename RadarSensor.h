#ifndef RADARSENSOR_H
#define RADARSENSOR_H

#include <Arduino.h>

typedef struct RadarTarget {
  float distance;  // mm
  float angle;     // radians
  float speed;     // cm/s
  int16_t x;       // mm
  int16_t y;       // mm
  bool detected;
} RadarTarget;

class RadarSensor {
  public:
    RadarSensor(HardwareSerial*, uint8_t rx_pin, uint8_t tx_pin, bool mode = false);
    void begin(unsigned long baud = 256000);
    bool update();
    RadarTarget getTarget(uint8_t index);
  private:
    HardwareSerial * radarSerial;
    uint8_t rx;
    uint8_t tx;
    bool multi_target = false;
    RadarTarget targets[3];  // three targets
    bool parseData(const uint8_t *buffer, size_t len);
};

#endif