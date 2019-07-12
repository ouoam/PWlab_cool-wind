#ifndef TEMP_h
#define TEMP_h

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define MAX_HISTORY 10

class Temp
{
private:
    OneWire oneWire = OneWire(2); // PORT For ds18b20 - upto 1/16
    DallasTemperature sensors = DallasTemperature(&oneWire);
    float tempHistory[MAX_HISTORY];
    uint8_t i_tempHistory = 0;
    uint64_t nextGetTemp = 0;

public:
    Temp()
    {
        sensors.setWaitForConversion(false);
        sensors.requestTemperatures();
        nextGetTemp = millis() + 750;
    }

    void setup()
    {
        sensors.begin();
    }

    void loop()
    {
        if (millis() > nextGetTemp)
        {
            tempHistory[i_tempHistory] = sensors.getTempCByIndex(0);
            i_tempHistory++;
            i_tempHistory %= MAX_HISTORY;
            sensors.requestTemperatures();
            nextGetTemp = millis() + 750;
        }
    }

    float getLastTemp()
    {
        return tempHistory[(i_tempHistory + MAX_HISTORY - 1) % MAX_HISTORY];
    }

    int32_t getEstimateTimeTo(int8_t pretemp)
    {
        int time = 750 * MAX_HISTORY;
        float diff = tempHistory[i_tempHistory] - getLastTemp();
        double changePerTime = diff / time;
        float nowToPre = getLastTemp() - pretemp;
        return nowToPre / changePerTime;
    }

    void printTemp()
    {
        // Serial.print("Temp C: ");
        // Serial.println(getLastTemp(), 4);
    }
};

Temp temp;

#endif