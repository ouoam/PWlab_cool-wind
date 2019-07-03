#include <Arduino.h>

#define COOL 4
#define BLOW 5
#define WATER 6

class IO
{
private:
public:
    void setup()
    {
        pinMode(COOL, OUTPUT);
        pinMode(BLOW, OUTPUT);
        pinMode(WATER, INPUT_PULLUP);
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
};
