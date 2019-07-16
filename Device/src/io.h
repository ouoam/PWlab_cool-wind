#ifndef IO_h
#define IO_h

#include <Arduino.h>

#define WATER_PIN   4

#define COOL_PIN    11
#define BLOW_PIN    12
#define BUZZER_PIN  13

#define SPEED1  8
#define SPEED2  9
#define SPEED3  10

class IO
{
private:
public:
    void setup()
    {
        pinMode(WATER_PIN, INPUT_PULLUP); /* *************** */

        pinMode(COOL_PIN, OUTPUT);
        pinMode(BLOW_PIN, OUTPUT);
        pinMode(BUZZER_PIN, OUTPUT);
        
        pinMode(SPEED1, OUTPUT);
        pinMode(SPEED2, OUTPUT);
        pinMode(SPEED3, OUTPUT);
    }

    void setCool(bool on)
    {
        digitalWrite(COOL_PIN, on);
    }

    void setBlow(bool on)
    {
        digitalWrite(BLOW_PIN, on);
    }

    bool isWater()
    {
        return digitalRead(WATER_PIN);
    }

    void setSpeed(uint8_t speed)
    {
        speed -= 1;
        digitalWrite(SPEED1, speed & B001);
        digitalWrite(SPEED2, speed & B010);
        digitalWrite(SPEED3, speed & B100);
    }
};

IO io;

#endif
