#ifndef SPEEDSERVICE_H
#define SPEEDSERVICE_H

#include <Arduino.h>
#include "IService.h"

class SpeedService : public IService {
public:
    // inPin: external interrupt-capable pin (e.g., 2 on Uno/Mega)
    // pulsesPerRev: hall pulses per wheel revolution (default 25)
    // wheelCircumferenceM: wheel circumference in meters (default 0.71 m)
    // isrEdge: RISING/FALLING/CHANGE (default FALLING for open-collector low pulse)
    // minPulseIntervalUs: debounce in ISR (ignored if 0)
    SpeedService(uint8_t inPin,
                 unsigned long updateIntervalMs,
                 uint16_t pulsesPerRev = 25,
                 float wheelCircumferenceM = 0.71f,
                 int isrEdge = RISING,
                 uint32_t minPulseIntervalUs = 500 /* ~0.5 ms */)
    : IService(updateIntervalMs),
      inPin_(inPin),
      PPR_(pulsesPerRev),
      CIRC_M_(wheelCircumferenceM),
      isrEdge_(isrEdge),
      minPulseIntervalUs_(minPulseIntervalUs) {}

    void begin() override {
        pinMode(inPin_, INPUT); // or INPUT_PULLUP if needed by your wiring
        s_instance_ = this;
        attachInterrupt(digitalPinToInterrupt(inPin_), &SpeedService::isrThunk_, isrEdge_);
        lastUpdateMs_ = millis();
    }

    void serviceLoop(unsigned long nowMs) override {
        if (nowMs - lastUpdateMs_ >= updateIntervalMs_) {
            // Snapshot and clear pulse count atomically
            noInterrupts();
            uint32_t pulses = pulseCount_;
            pulseCount_ = 0;
            interrupts();

            unsigned long dtMs = nowMs - lastUpdateMs_;
            lastUpdateMs_ = nowMs;

            float kmh = 0.0f;
            if (dtMs > 0 && pulses >= 0) {
                // revs = pulses / PPR
                // meters = revs * CIRC
                // m/s = meters / (dtMs/1000)
                // km/h = m/s * 3.6
                float revs   = (float)pulses / (float)PPR_;
                float meters = revs * CIRC_M_;
                float mps    = meters / ((float)dtMs / 1000.0f);
                kmh          = mps * 3.6f;
            }

            // Round to whole km/h
            int kmhInt = (int)(kmh + 0.5f);
            if ((float)kmhInt != displayData_.value) {
              displayData_.value = (float)kmhInt;
              displayData_.shouldUpdate = true;
            }
        }
    }

private:
    // ----- ISR -----
    static void isrThunk_() {
        if (s_instance_) s_instance_->onPulse_();
    }

    inline void onPulse_() {
        // Optional simple debounce using micros()
        if (minPulseIntervalUs_ > 0) {
            uint32_t now = micros();
            uint32_t delta = now - lastPulseUs_;  // wraps OK for unsigned
            if (delta < minPulseIntervalUs_) return;
            lastPulseUs_ = now;
        }
        ++pulseCount_;
    }

    // ----- Config & state -----
    const uint8_t  inPin_;
    const uint16_t PPR_;
    const float    CIRC_M_;
    const int      isrEdge_;
    const uint32_t minPulseIntervalUs_;

    volatile uint32_t pulseCount_ = 0;     // up to ~1000 pulses/s is fine
    volatile uint32_t lastPulseUs_ = 0;

    unsigned long lastUpdateMs_ = 0;

    static SpeedService* s_instance_;
};

// Static member definition
inline SpeedService* SpeedService::s_instance_ = nullptr;

#endif // SPEEDSERVICE_H
