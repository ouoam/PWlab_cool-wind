#ifndef DISPLAY_h
#define DISPLAY_h

#include <Arduino.h>

#include "Nextion.h"

#include "io.h"

uint8_t page;
uint8_t fanspeed;

unsigned long valRemain = 0;
unsigned long startTime = 0;
unsigned long nextChangeTime = 0;
bool isPauseVal = false;
bool isDeforceVal = false;

NexPage settingPg = NexPage(0, 0, "main");
NexPage workingPg = NexPage(2, 0, "working");

// ---------- page 0 ----------
NexVariable valTime = NexVariable(0, 9, "valTime", "main");
NexVariable valTemp = NexVariable(0, 10, "valTemp", "main");
NexVariable valStatus = NexVariable(0, 13, "valStatus", "main");
NexVariable valETA = NexVariable(0, 14, "valETA", "main");
NexVariable valButton = NexVariable(0, 15, "valButton", "main");

NexPicture startBtn = NexPicture(0, 3, "p2");

NexText textDebug = NexText(0, 18, "textDebug");

void startBtnCallBack(void *ptr)
{
    workingPg.show();
    page = 2;
    io.setBlow(true);
    startTime = millis();
    nextChangeTime = 0;
    uint32_t timeMin = 0;
    while (!valTime.getValue(&timeMin));
    valRemain = timeMin * 60 * 1000;
}

// ---------- page 2 ----------

NexNumber fanSpeed = NexNumber(2, 5, "fanSpeed");
NexText remainTime = NexText(2, 6, "remainTime");

NexHotspot timeMinus = NexHotspot(2, 7, "timeMinus");
NexHotspot timePlus = NexHotspot(2, 8, "timePlus");
NexHotspot fanMinus = NexHotspot(2, 9, "fanMinus");
NexHotspot fanPlus = NexHotspot(2, 10, "fanPlus");
NexHotspot hDefrost = NexHotspot(2, 11, "hDefrost");
NexHotspot hPause = NexHotspot(2, 12, "hPause");
NexHotspot hStop = NexHotspot(2, 13, "hStop");

NexVariable isPause = NexVariable(2, 3, "isPause", "working");
NexVariable isDefrost = NexVariable(2, 4, "isDefrost", "working");

void timeMinusCallBack(void *ptr)
{
    if (valRemain > 61ul * 1000) {
        valRemain -= 60ul * 1000;
        nextChangeTime = 0;
    }
}

void timePlusCallBack(void *ptr)
{
    if (valRemain < 99ul * 60 * 1000) {
        valRemain += 60ul * 1000;
        nextChangeTime = 0;
    }
}

void fanMinusCallBack(void *ptr)
{
    fanspeed--;
    if (fanspeed < 1)
        fanspeed = 1;
    fanSpeed.setValue(fanspeed);
    io.setSpeed(fanspeed);
}

void fanPlusCallBack(void *ptr)
{
    fanspeed++;
    if (fanspeed > 8)
        fanspeed = 8;
    fanSpeed.setValue(fanspeed);
    io.setSpeed(fanspeed);
}

void hDefrostCallBack(void *ptr)
{
    isDeforceVal = !isDeforceVal;
    isDefrost.setValue(isDeforceVal);
    nextChangeTime = 0;

    if (isDeforceVal) { // On deforce
        io.setBlow(true);
        fanspeed = 6;
        fanSpeed.setValue(fanspeed);
        io.setSpeed(fanspeed);
        if (!isPauseVal) {
            valRemain -= millis() - startTime;
        }
    } else { // Off deforce
        if (!isPauseVal) {
            io.setBlow(true);
            startTime = millis();
        } else {
            io.setBlow(false);
        }
    }
}

void hPauseCallBack(void *ptr)
{
    isPauseVal = !isPauseVal;
    isPause.setValue(isPauseVal);
    nextChangeTime = 0;

    if (isPauseVal) { // On pause
        if (!isDeforceVal) {
            io.setBlow(false);
            valRemain -= millis() - startTime;
        } else {
            io.setBlow(true);
        }
    } else { // Off pause
        if (!isDeforceVal) {
            io.setBlow(true);
            startTime = millis();
        }
    }
}

void hStopCallBack(void *ptr)
{
    settingPg.show();
    page = 0;
    io.setBlow(false);
}


NexTouch *nex_listen_list[] =
    {
        &startBtn,
        &timeMinus,
        &timePlus,
        &fanMinus,
        &fanPlus,
        &hDefrost,
        &hPause,
        &hStop,
        NULL};

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

        // ---------- page 0 ----------
        startBtn.attachPush(startBtnCallBack, &startBtn);

        // ---------- page 1 ----------

        //   -----  Time speed  -----
        timeMinus.attachPush(timeMinusCallBack, &timeMinus);
        timePlus.attachPush(timePlusCallBack, &timePlus);

        //   -----  Fan speed  -----
        fanMinus.attachPush(fanMinusCallBack, &fanMinus);
        fanPlus.attachPush(fanPlusCallBack, &fanPlus);

        //   -----  deforce pause stop  -----
        hDefrost.attachPush(hDefrostCallBack, &hDefrost);
        hPause.attachPush(hPauseCallBack, &hPause);
        hStop.attachPush(hStopCallBack, &hStop);

        page = 0;
        fanspeed = 1;
    }

    void loop()
    {
        nexLoop(nex_listen_list);
        if (page == 2) {
            if (millis() >= nextChangeTime && ((!isPauseVal && !isDeforceVal) || nextChangeTime == 0)) {
                unsigned long use = millis() - startTime;
                if (isPauseVal || isDeforceVal) use = 0;
                long remain = (long)valRemain - (long)use;
                if (remain > 0) {
                    remain /= 1000;
                    String out = "";
                    out += remain / 60;
                    out += ":";
                    out += (remain % 60 >= 10) ? "" : "0";
                    out += remain % 60;
                    remainTime.setText(out.c_str());

                    startTime += use;
                    valRemain -= use;

                    nextChangeTime = millis() + (valRemain % 1000);
                } else {
                    settingPg.show();
                    page = 0;
                    io.setSpeed(1);
                    io.setBlow(false);
                    
                }
            }
        }
        
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

Display display;

#endif
