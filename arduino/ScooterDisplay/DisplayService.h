#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

#include <Arduino.h>
#include <UTFT.h>
// --- Fonts you already use ---
#include "fonts/SevenSeg_XXXL_Num.c"
#include "fonts/Grotesk24x48.c"
#include "fonts/symbols.c"     

#define SCREEN_WIDTH 479
#define SCREEN_HEIGHT 319
#define SPEED_BOX_X 170
#define SPEED_BOX_Y 100
#define SPEED_BOX_W 122
#define SPEED_BOX_H 99

#define VOLT_BOX_X 25
#define VOLT_BOX_Y 30
#define VOLT_BOX_W 100
#define VOLT_BOX_H 46

#define LEFT_BLINKR_X 10
#define RIGHT_BLINKR_X 410
#define BLINKR_Y 110
#define BLINKR_W 80
#define BLINKR_H 110

extern uint8_t BigFont[]; // provided by UTFT package

class DisplayService {
public:
    // Pass pointers to the services' DisplayData (speed in km/h as integer, battery in volts as float)
    void begin(DisplayData* speedData, DisplayData* batteryData, DisplayData* leftBlinkerData, DisplayData* rightBlinkerData) {
        speed   = speedData;
        battery = batteryData;
        leftBlinker = leftBlinkerData;
        rightBlinker = rightBlinkerData;

        myGLCD.InitLCD();
        myGLCD.clrScr();

        // Background
        myGLCD.setColor(0, 0, 100);
        myGLCD.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Default text colors
        myGLCD.setBackColor(0, 0, 100);
        myGLCD.setColor(255, 255, 255);

        // ----- Static layout -----
        myGLCD.setFont(Grotesk24x48);
        myGLCD.print("88.8", VOLT_BOX_X, VOLT_BOX_Y);
        myGLCD.print(" V", VOLT_BOX_X + 80, VOLT_BOX_Y);
        
        // Speed placeholder (big digits)
        myGLCD.setFont(SevenSeg_XXXL_Num);
        myGLCD.print("88", SPEED_BOX_X, SPEED_BOX_Y);

        // Units under speed
        myGLCD.setFont(Grotesk24x48);
        myGLCD.print("km/h", 185, 210);

        // Trip / Total placeholders
        myGLCD.setFont(BigFont);
        myGLCD.print("Trip:",  15, 270);
        myGLCD.print("888888 km", 15, 295);
        myGLCD.print("Total:", 310, 270);
        myGLCD.print("888888 km", 310, 295);

        // Clear voltage area so the first update paints a clean field
        clearRect(VOLT_BOX_X, VOLT_BOX_Y, VOLT_BOX_W, VOLT_BOX_H);
    }

    void update() {
        if (speed && speed->shouldUpdate) {
            drawSpeed(speed->value);
            speed->shouldUpdate = false;
        }
        if (battery && battery->shouldUpdate) {
            drawBattery(battery->value);
            battery->shouldUpdate = false;
        }
        if (leftBlinker && leftBlinker->shouldUpdate) {
            drawLeftBlinker(bool(leftBlinker->value));
            leftBlinker->shouldUpdate = false;
        }
        if (rightBlinker && rightBlinker->shouldUpdate) {
            drawRightBlinker(bool(rightBlinker->value));
            rightBlinker->shouldUpdate = false;
        }
    }

private:
    UTFT myGLCD = UTFT(ILI9486, 38, 39, 40, 41);
    DisplayData* speed   = nullptr; 
    DisplayData* battery = nullptr; 
    DisplayData* leftBlinker = nullptr;
    DisplayData* rightBlinker = nullptr;
    
    char speedbuf[4];
    char voltagebuf[5];

    inline void clearRect(int x, int y, int w, int h) {
        myGLCD.setColor(0, 0, 100); 
        myGLCD.fillRect(x, y, x + w, y + h);
        myGLCD.setColor(255, 255, 255); 
        myGLCD.setBackColor(0, 0, 100);
    }

    void drawSpeed(float val) {
        clearRect(SPEED_BOX_X, SPEED_BOX_Y, SPEED_BOX_W, SPEED_BOX_H);
        int kmh = (int)(val + 0.5f); // should already be rounded upstream, but safe

        itoa(kmh, speedbuf, 10); 
        myGLCD.setFont(SevenSeg_XXXL_Num);
        myGLCD.print(speedbuf, SPEED_BOX_X, SPEED_BOX_Y);
    }

    void drawBattery(float volts) {
        clearRect(VOLT_BOX_X, VOLT_BOX_Y, VOLT_BOX_W, VOLT_BOX_H);

        myGLCD.setFont(Grotesk24x48);
        dtostrf(volts, 0, 1, voltagebuf);  // "72.4"
        myGLCD.print(voltagebuf, VOLT_BOX_X, VOLT_BOX_Y);
    }
    
    void drawLeftBlinker(bool on) {
        clearRect(LEFT_BLINKR_X, BLINKR_Y, BLINKR_W, BLINKR_H);
        if (on) {
            myGLCD.setColor(255, 255, 0);
            myGLCD.setBackColor(0, 0, 100);
            myGLCD.setFont(symbols);
            myGLCD.print("<", LEFT_BLINKR_X, BLINKR_Y);
        }
    }

    void drawRightBlinker(bool on) {
        clearRect(RIGHT_BLINKR_X, BLINKR_Y, BLINKR_W, BLINKR_H);
        if (on) {
            myGLCD.setColor(255, 255, 0);
            myGLCD.setBackColor(0, 0, 100);
            myGLCD.setFont(symbols);
            myGLCD.print(">", RIGHT_BLINKR_X, BLINKR_Y);
        }
      }
    
};

#endif // DISPLAY_SERVICE_H
