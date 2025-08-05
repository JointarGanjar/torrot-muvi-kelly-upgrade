// Main Arduino sketch structure for Scooter Display System
// Implements modular services: Speed, Battery, Blinkers, Display
// Each service derives from a common IService interface

#include <Arduino.h>
#include <UTFT.h>
#include "IService.h"
#include "SpeedService.h"
#include "BatteryService.h"
#include "DisplayService.h"
#include "BlinkerUnit.h"
#include "BlinkerManager.h"

// --- Pin definition ---
#define voltagePin A0
#define speedPin 2
#define leftBlinkerSignal 10
#define rightBlinkerSignal 11
#define leftBlinkerOutput 8
#define rightBlinkerOutput 9

// --- Global Instances ---
SpeedService speedService(speedPin, /*updateMs=*/500);
BatteryService batteryService(voltagePin, /*updateMs=*/2000);
DisplayService displayService;
BlinkerUnit leftBlinker(leftBlinkerSignal, leftBlinkerOutput, /*updateMs=*/300);
BlinkerUnit rightBlinker(rightBlinkerSignal, rightBlinkerOutput, /*updateMs=*/300);
BlinkerManager blMgr(leftBlinker, rightBlinker, /*updateMs=*/300);

// --- Main setup and loop ---
void setup() {
    speedService.begin();
    batteryService.begin();
    leftBlinker.begin();
    rightBlinker.begin();
    blMgr.beginTimerBase();

    displayService.begin(
        speedService.getDisplayData(),
        batteryService.getDisplayData(),
        leftBlinker.getDisplayData(),
        rightBlinker.getDisplayData()
    );
}

void loop() {
    unsigned long now = millis();

    speedService.serviceLoop(now);
    batteryService.serviceLoop(now);
    leftBlinker.serviceLoop(now);
    rightBlinker.serviceLoop(now);
    blMgr.loop(now);

    displayService.update();
}
