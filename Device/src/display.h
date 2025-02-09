#ifndef DISPLAY_h
#define DISPLAY_h

#include <Arduino.h>

#include "Nextion.h"

#include "io.h"
#include "temp.h"

uint8_t page = 0;
uint8_t fanspeed = 1;

unsigned long valRemain = 0;
unsigned long startTime = 0;
unsigned long nextChangeTime = 0;
bool isPauseVal = false;
bool isDeforceVal = false;
bool isWaterLock = false;
bool willChangeStatus = false;

NexPage settingPg = NexPage(0, 0, "main");
NexPage workingPg = NexPage(2, 0, "working");

// ---------- page 0 ----------
NexVariable valTime = NexVariable(0, 9, "valTime", "main");
NexVariable valTemp = NexVariable(0, 10, "valTemp", "main");
NexVariable valStatus = NexVariable(0, 13, "valStatus", "main");
NexVariable valETA = NexVariable(0, 14, "valETA", "main");
NexVariable valButton = NexVariable(0, 15, "valButton", "main");

NexPicture startBtn = NexPicture(0, 3, "p2");
NexHotspot clickStatus = NexHotspot(0, 20, "clickStatus");

NexText textDebug = NexText(0, 18, "textDebug");

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

// ---------- callback page 0 ----------

void startBtnCallBack(void *ptr)
{
    
    page = 2;
    fanspeed = 1;
    io.setBlow(true);
    io.setCool(HIGH);
    io.setSpeed(fanspeed);
    nextChangeTime = 0;
    
    isDeforceVal = false;
    isPauseVal = false;
    isWaterLock = false;
    isDefrost.setValue(isDeforceVal);
    isPause.setValue(isPauseVal);

    uint32_t timeMin = 0;
    while (!valTime.getValue(&timeMin));
    valRemain = timeMin * 60 * 1000;

    while (!workingPg.show());
    startTime = millis();
    io.sound();
}

void clickStatusCallBack(void *ptr)
{
    if (!io.isWater()) {
        isWaterLock = false;
        io.setCool(HIGH);
    }
}

// ---------- callback page 2 ----------

void timeMinusCallBack(void *ptr)
{
    if (valRemain > 61ul * 1000) {
        valRemain -= 60ul * 1000;
        nextChangeTime = 0;
    }
    io.sound();
}

void timePlusCallBack(void *ptr)
{
    if (valRemain < 99ul * 60 * 1000) {
        valRemain += 60ul * 1000;
        nextChangeTime = 0;
    }
    io.sound();
}

void fanMinusCallBack(void *ptr)
{
    fanspeed--;
    if (fanspeed < 1)
        fanspeed = 1;
    fanSpeed.setValue(fanspeed);
    io.setSpeed(fanspeed);
    io.sound();
}

void fanPlusCallBack(void *ptr)
{
    fanspeed++;
    if (fanspeed > 8)
        fanspeed = 8;
    fanSpeed.setValue(fanspeed);
    io.setSpeed(fanspeed);
    io.sound();
}

void hDefrostCallBack(void *ptr)
{
    if (!isWaterLock) {
        isDeforceVal = !isDeforceVal;
        isDefrost.setValue(isDeforceVal);
        nextChangeTime = 0;

        if (isDeforceVal) { // On deforce
            io.setBlow(true);
            if (fanspeed < 6) {
                fanspeed = 6;
                fanSpeed.setValue(fanspeed);
                io.setSpeed(fanspeed);
            }
            if (!isPauseVal) {
                valRemain -= millis() - startTime;
            }
            io.setCool(LOW);
        } else { // Off deforce
            if (!isPauseVal) {
                io.setBlow(true);
                startTime = millis();
            } else {
                io.setBlow(false);
            }
            io.setCool(HIGH);
        }
    } else {
        if (!io.isWater()) {
            nextChangeTime = 0;
            willChangeStatus = true;

            if (!isPauseVal && !isDeforceVal) {
                io.setBlow(true);
                startTime = millis();
            } else {
                io.setBlow(false);
            }
            io.setCool(HIGH);
        }
    }
    io.sound();
}

void hPauseCallBack(void *ptr)
{
    isPauseVal = !isPauseVal;
    isPause.setValue(isPauseVal);
    nextChangeTime = 0;

    if (isPauseVal) { // On pause
        if (!isDeforceVal && !isWaterLock) {
            io.setBlow(false);
            valRemain -= millis() - startTime;
        } else if (isDeforceVal && !isWaterLock) {
            io.setBlow(true);
        } else {
            io.setBlow(false);
        }
    } else { // Off pause
        if (!isDeforceVal && !isWaterLock) {
            io.setBlow(true);
            startTime = millis();
        }
    }
    io.sound();
}

void hStopCallBack(void *ptr)
{
    settingPg.show();
    page = 0;
    fanspeed = 1;
    io.setBlow(false);
    io.setCool(HIGH);
    io.setSpeed(fanspeed);
    io.sound();
}


NexTouch *nex_listen_list0[] =
    {
        &startBtn,
        &clickStatus,
        NULL};

NexTouch *nex_listen_list2[] =
    {
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
    uint8_t lastValStatus = 0;
    uint8_t lastValButton = 0;
    int32_t lastETAtime = 0;

public:
    Display(/* args */)
    {
        // init
    }

    void setup()
    {
        nexInit();

        // Change band rate to 512000
        sendCommand("baud=512000");
        recvRetCommandFinished();
        nexSerial.end();
        nexSerial.begin(512000);
        sendCommand("");

        // ---------- page 0 ----------
        startBtn.attachPush(startBtnCallBack, &startBtn);
        clickStatus.attachPush(clickStatusCallBack, &clickStatus);

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
        if (page == 0) {
            nexLoop(nex_listen_list0);
            if (nextChangeTime <= millis()) {
                int temppp = getPreTemp();

                if (temppp >= temp.getLastTemp())
                {
                    setStatus(isWaterLock ? 3 : 2);
                    setButton(isWaterLock ? B0 : B1);
                }
                else
                {
                    setETAtime(temp.getEstimateTimeTo(temppp) / 1000);
                    if (getStatus() > 1) setStatus(isWaterLock ? 3 : 1);
                    setButton(B0);
                }
                if (io.isWater() && !isWaterLock) {
                    isWaterLock = true;
                    io.setCool(LOW);
                    setStatus(3);
                }
                nextChangeTime = millis() + 200;
            }
        }

        if (page == 2) {
            nexLoop(nex_listen_list2);
            if (io.isWater() && !isWaterLock) {
                isWaterLock = true;
                setStatus(3);

                nextChangeTime = 0;
                io.setBlow(LOW);
                if (!isPauseVal && !isDeforceVal) {
                    valRemain -= millis() - startTime;
                }
                io.setCool(LOW);
            }

            if (!io.isWater() && isWaterLock && nextChangeTime == 0 && willChangeStatus) {
                setStatus(2);
                isWaterLock = false;
                willChangeStatus = false;
            }

            if (millis() >= nextChangeTime && ((!isPauseVal && !isDeforceVal && !isWaterLock) || nextChangeTime == 0)) {
                unsigned long use = millis() - startTime;
                if (isPauseVal || isDeforceVal || isWaterLock) use = 0;
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
                    io.sound(3);
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

    uint8_t getStatus()
    {
        uint32_t number;
        valStatus.getValue(&number);
        return number;
    }

    void setETAtime(int32_t time)
    {
        if (time > 999)
            time = 999;
        else if (time < 0)
            time = 0;
        if (lastETAtime == time) return;
        lastETAtime = time;
        valETA.setValue(time);
    }

    void setStatus(uint8_t status)
    {
        if (lastValStatus == status) return;
        lastValStatus = status;
        valStatus.setValue(status);
    }

    void setButton(uint8_t button)
    {
        if (lastValButton == button) return;
        lastValButton = button;
        valButton.setValue(button);
    }

    void setDebug(const char str[])
    {
        textDebug.setText(str);
    }
};

Display display;

#endif
