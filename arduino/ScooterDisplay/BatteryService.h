#ifndef BATTERYSERVICE_H
#define BATTERYSERVICE_H

#include <Arduino.h>
#include "IService.h"

class BatteryService : public IService {
public:
    BatteryService(int inputPin,
                   unsigned long updateIntervalMs)
        : IService(updateIntervalMs),
        analogPin(inputPin) {}

    void begin() override {
        pinMode(analogPin, INPUT);
    }

    void serviceLoop(unsigned long now) override {
        if (now - lastUpdate >= updateIntervalMs_) {
              // ---- Read & compute corrected voltage ----
            int   raw         = readAveraged(analogPin);
            float voltageOut  = (raw / 1023.0f) * referenceVoltage; // 0..5V at divider output
            float voltageIn   = voltageOut * dividerFactor;         // scaled back up
            float corrected   = voltageIn * correctionFactor;       // calibrated

            if (corrected != displayData_.value) {
              displayData_.value = corrected;
              displayData_.shouldUpdate = true;
            }
            lastUpdate = now;
        }
    }


private:
    int readAveraged(int pin) {
      long sum = 0;
      for (int i = 0; i < 6; i++) {
        sum += analogRead(analogPin);
      }
      return (int)(sum / 6);
    }

    const int analogPin;
    const float referenceVoltage  = 5.0;
    const float dividerFactor     = (147000.0 + 10000.0) / 10000.0;   // ≈ 15.7
    const float correctionFactor  = 49.1 / 46.4;
    
    unsigned long lastUpdate = 0;
};

#endif // BATTERYSERVICE_H
