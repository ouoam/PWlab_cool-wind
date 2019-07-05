#include <Arduino.h>

#include "Nextion.h"

NexVariable valTime = NexVariable(0, 9, "valTime", "main");
NexVariable valTemp = NexVariable(0, 10, "valTemp", "main");
NexVariable valStatus = NexVariable(0, 13, "valStatus", "main");
NexVariable valETA = NexVariable(0, 14, "valETA", "main");
NexVariable valButton = NexVariable(0, 15, "valButton", "main");

NexText textDebug = NexText(0, 18, "textDebug");

class Display
{
private:
    int8_t lastPreTemp = 0;

public:
    Display(/* args */)
    {
        // init
    }

    void setup()
    {
        nexInit();
    }

    void loop()
    {
    }

    int8_t getPreTemp()
    {
        uint32_t number;
        if (valTemp.getValue(&number))
        {
            lastPreTemp = number;
        }
        return lastPreTemp;
    }

    int8_t getTime()
    {
        uint32_t number;
        valTime.getValue(&number);
        return number;
    }

    void setETAtime(int32_t time)
    {
        if (time > 999)
            time = 999;
        else if (time < 0)
            time = 0;
        valETA.setValue(time);
    }

    uint8_t getStatus()
    {
        uint32_t number;
        valStatus.getValue(&number);
        return number;
    }

    void setStatus(uint8_t status)
    {
        valStatus.setValue(status);
    }

    void setButton(uint8_t button)
    {
        valButton.setValue(button);
    }

    void setDebug(String str)
    {
        textDebug.setText(str.c_str());
    }
};