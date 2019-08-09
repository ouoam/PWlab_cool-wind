#ifndef IO_h
#define IO_h

#include <Arduino.h>

#define WATER_PIN   4

#define COOL_PIN    11
#define BLOW_PIN    12
#define BUZZER_PIN  13
#define WATERFULL_PIN   PC0

#define SPEED1  8
#define SPEED2  9
#define SPEED3  10

class IO
{
private:
    uint64_t willOffBuzzTime = 0 - 1;
    uint64_t willOnBuzzTime = 0 - 1;
    uint8_t round = 0;
public:
    void setup()
    {
        pinMode(WATER_PIN, INPUT_PULLUP); /* *************** */

        pinMode(COOL_PIN, OUTPUT);
        digitalWrite(COOL_PIN, LOW);

        pinMode(BLOW_PIN, OUTPUT);
        digitalWrite(BLOW_PIN, LOW);
        
        pinMode(BUZZER_PIN, OUTPUT);
        digitalWrite(BUZZER_PIN, LOW);

        pinMode(WATERFULL_PIN, OUTPUT);
        digitalWrite(WATERFULL_PIN, LOW);
        
        // SPEED CONTROL
        pinMode(SPEED1, OUTPUT);
        digitalWrite(SPEED1, LOW);

        pinMode(SPEED2, OUTPUT);
        digitalWrite(SPEED2, LOW);

        pinMode(SPEED3, OUTPUT);
        digitalWrite(SPEED3, LOW);
    }

    void loop()
    {
        if (millis() >= willOnBuzzTime) {
            digitalWrite(BUZZER_PIN, HIGH);
            willOffBuzzTime = millis() + 300;
            willOnBuzzTime = 0 - 1;
        }
        if (millis() >= willOffBuzzTime) {
            digitalWrite(BUZZER_PIN, LOW);
            willOffBuzzTime = 0 - 1;
            if (round > 1) {
                round--;
                willOnBuzzTime = millis() + 200;
            }
        }
        
    }

    void setCool(bool on)
    {
        digitalWrite(COOL_PIN, !on);
    }

    void setBlow(bool on)
    {
        digitalWrite(BLOW_PIN, on);
    }

    void sound(uint8_t round = 1)
    {
        willOnBuzzTime = 0;
        this->round = round;
    }

    bool isWater()
    {
        return !digitalRead(WATER_PIN); // Pullup now
    }

    void setSpeed(uint8_t speed)
    {
        speed -= 1;
        digitalWrite(SPEED1, speed & B001);
        digitalWrite(SPEED2, speed & B010);
        digitalWrite(SPEED3, speed & B100);
    }

    void setWaterFullLock(bool on) {
        digitalWrite(COOL_PIN, on);
    }
};

IO io;

#endif
