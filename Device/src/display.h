#include <Arduino.h>

#include "Nextion.h"

NexNumber n0 = NexNumber(0, 8, "n0");
NexNumber n1 = NexNumber(0, 9, "n1");

class Display
{
private:
    uint8_t page = 0;
    float temp = 0;
    int32_t time = 0;

public:
    Display(/* args */)
    {
        // init
    }

    void setup()
    {
        pinMode(11, INPUT_PULLUP);
        pinMode(12, INPUT_PULLUP);

        nexInit();
    }

    void loop()
    {
    }

    void setPage(uint8_t page)
    {
        this->page = page;
    }

    uint8_t getPage()
    {
        return page;
    }

    void setTemp(float temp)
    {
        this->temp = temp;

        n0.setValue(temp);
    }

    void setTime(int32_t time)
    {
        this->time = time;
    }

    void setETAtime(int32_t time)
    {
        if (time > 999)
            time = 999;
        else if (time < 0)
            time = 0;
        n1.setValue(time);
    }

    String printStatus()
    {
        String out = "";
        out += "Page:";
        out += page;
        out += " Temp:";
        out += temp;
        out += " ETA:";
        out += time;

        return out;
    }
};