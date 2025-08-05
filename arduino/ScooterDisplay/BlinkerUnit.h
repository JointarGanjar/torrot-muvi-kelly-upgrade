#ifndef BLINKER_UNIT_H
#define BLINKER_UNIT_H

#include <Arduino.h>
#include "IService.h"

class BlinkerUnit : public IService {
public:
    BlinkerUnit(int inputPin, int outputPin, unsigned long updateIntervalMs, bool activeHigh = false)
      : IService(updateIntervalMs), inPin(inputPin), outPin(outputPin), ACTIVE_HIGH(activeHigh) {}

    void begin() override {
        pinMode(inPin, INPUT);
        pinMode(outPin, OUTPUT);
        setOutput(false);            // ensure off
    }

    void serviceLoop(unsigned long now) override {
        if (now - lastInputCheck >= updateIntervalMs_) {
            bool newState = digitalRead(inPin);
            if (newState != active) {
                active = newState;
                displayData_.shouldUpdate = true;
                displayData_.value = active ? 1.0 : 0.0;
            }
            lastInputCheck = now;
        }
    }

    
    // Called by ISR when this unit is active
    void onBlinkTimerTick() {
        if (!active) return;
        blinkState = !blinkState;
        setOutput(blinkState);
    }

    // Force output off (used by manager when deselecting)
    void forceOff() {
        blinkState = false;
        setOutput(false);
    }

    bool isActiveInput() const { return active; }

private:
    void setOutput(bool on) {
        digitalWrite(outPin, (on == ACTIVE_HIGH) ? HIGH : LOW);
    }

    const int inPin, outPin;
    const bool ACTIVE_HIGH;
    unsigned long lastInputCheck = 0;
    bool active = false;      // input request
    bool blinkState = false;  // output phase
};

#endif // BLINKER_UNIT_H
