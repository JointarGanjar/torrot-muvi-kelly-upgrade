#ifndef BLINKER_MANAGER_H
#define BLINKER_MANAGER_H

#include <Arduino.h>
#include "BlinkerUnit.h"

volatile BlinkerUnit* g_activeUnit = nullptr;

ISR(TIMER1_COMPA_vect) {
    BlinkerUnit* u = (BlinkerUnit*)g_activeUnit;
    if (u) u->onBlinkTimerTick();
}

class BlinkerManager {
public:
    BlinkerManager(BlinkerUnit& left, BlinkerUnit& right, unsigned long loopInterval)
      : L(left), R(right), loopInterval(loopInterval) {}

    void beginTimerBase() {
        noInterrupts();
        TCCR1A = 0; TCCR1B = 0; TCNT1 = 0;
        OCR1A = 7812;               // 1 Hz toggle (adjust as needed)
        TCCR1B |= (1 << WGM12);      // CTC
        TIMSK1 |= (1 << OCIE1A);     // enable compare A
        interrupts();
        stopTimer();
    }

    void loop(unsigned long now) {
        // debounce window
        if (now - lastCheck < 200) return;
        lastCheck = now;

        bool l = L.isActiveInput();
        bool r = R.isActiveInput();

        const BlinkerUnit* want =
            (l && !r) ? &L :
            (r && !l) ? &R :
            nullptr; // both or none

        if (want != selected) {
            switchTo((BlinkerUnit*)want);
        }
    }

private:
    void switchTo(BlinkerUnit* next) {
        noInterrupts();
        stopTimer();
        g_activeUnit = nullptr;
        interrupts();

        // ensure both outputs off
        L.forceOff();
        R.forceOff();

        selected = next;

        if (selected) {
            noInterrupts();
            g_activeUnit = selected;
            startTimer();
            interrupts();
        }
    }

    void startTimer() {
        TCNT1 = 0;
        TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // prescaler 1024
    }
    void stopTimer() {
        TCCR1B = (1 << WGM12); // prescaler cleared → stopped
    }

    BlinkerUnit& L;
    BlinkerUnit& R;
    BlinkerUnit* selected = nullptr;
    unsigned long loopInterval;
    unsigned long lastCheck = 0;
};

#endif // BLINKER_MANAGER_H
