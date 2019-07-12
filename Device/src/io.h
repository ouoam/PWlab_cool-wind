#ifndef IO_h
#define IO_h

#include <Arduino.h>

#define COOL 4
#define BLOW 5
#define WATER 6

#define SPEED1 8
#define SPEED2 9
#define SPEED3 10

class IO
{
private:
public:
    void setup()
    {
        pinMode(COOL, OUTPUT);
        pinMode(BLOW, OUTPUT);
        pinMode(WATER, INPUT_PULLUP);
        pinMode(SPEED1, OUTPUT);
        pinMode(SPEED2, OUTPUT);
        pinMode(SPEED3, OUTPUT);
    }

    void setCool(bool on)
    {
        digitalWrite(COOL, on);
    }

    void setBlow(bool on)
    {
        digitalWrite(BLOW, on);
    }

    bool isWater()
    {
        return digitalRead(WATER);
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
