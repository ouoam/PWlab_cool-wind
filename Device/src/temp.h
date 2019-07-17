#ifndef TEMP_h
#define TEMP_h

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define SENSOR_PRE_TEMP_PIN 2
#define SENSOR_EX_TEMP_PIN  3

#define MAX_HISTORY 10

class Temp
{
private:
    OneWire oneWirePre = OneWire(SENSOR_PRE_TEMP_PIN); // PORT For ds18b20 - upto 1/16
    OneWire oneWireEx  = OneWire(SENSOR_EX_TEMP_PIN);
    DallasTemperature sensorsPre = DallasTemperature(&oneWirePre);
    DallasTemperature sensorsEx  = DallasTemperature(&oneWireEx);
    DeviceAddress DeviceAddressPre;
    DeviceAddress DeviceAddressEx;

    float tempHistory[MAX_HISTORY];
    float lastExTemp;
    uint8_t i_tempHistory = 0;
    uint64_t nextGetTemp = 0;

public:
    Temp()
    {
        sensorsPre.setWaitForConversion(false);
        sensorsEx.setWaitForConversion(false);
        
        nextGetTemp = millis() + 375;
    }

    void setup()
    {
        sensorsPre.begin();
        sensorsEx.begin();

        sensorsPre.getAddress(DeviceAddressPre, 0);
        sensorsEx.getAddress(DeviceAddressEx, 0);

        sensorsPre.setResolution(DeviceAddressPre, 11);
        sensorsEx.setResolution(DeviceAddressEx, 11);

        sensorsPre.requestTemperatures();
        sensorsEx.requestTemperatures();
    }

    void loop()
    {
        if (millis() > nextGetTemp)
        {
            tempHistory[i_tempHistory] = sensorsPre.getTempCByIndex(0);
            i_tempHistory++;
            i_tempHistory %= MAX_HISTORY;
            sensorsPre.requestTemperatures();

            lastExTemp = sensorsEx.getTempCByIndex(0);
            sensorsEx.requestTemperatures();

            nextGetTemp += 375;
        }
    }

    float getLastTemp()
    {
        return tempHistory[(i_tempHistory + MAX_HISTORY - 1) % MAX_HISTORY];
    }

    float getLastExTemp()
    {
        return lastExTemp;
    }

    int32_t getEstimateTimeTo(int8_t pretemp)
    {
        int time = 750 * MAX_HISTORY;
        float diff = tempHistory[i_tempHistory] - getLastTemp();
        double changePerTime = diff / time;
        float nowToPre = getLastTemp() - pretemp;
        return nowToPre / changePerTime;
    }
};

Temp temp;

#endif