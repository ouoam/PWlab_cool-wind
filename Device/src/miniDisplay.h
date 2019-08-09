#ifndef MINI_DISPLAY_h
#define MINI_DISPLAY_h

#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>

#include "u8g2_font_logisoso42_tc.h"
#include "u8g2_font_logisoso24_tc.h"

#include "temp.h"
#include "io.h"

class MINI_DISPLAY
{
private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2 = U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    float lastShowTemp = -128;
    bool lastWaterState = false;
public:
    void setup()
    {
        u8g2.begin();
    }

    void loop()
    {
        bool nowWaterState = io.isWater();
        float tempC = temp.getLastExTemp();
        if (lastShowTemp == tempC && nowWaterState == lastWaterState) return;
        
        lastShowTemp = tempC;

        bool neg = tempC < 0;
        tempC = neg ? -tempC : tempC;
        uint8_t digit2 = (int)(tempC / 10) % 10;
        uint8_t digit1 = (int)tempC % 10;
        uint8_t point1 = (int)(tempC * 10) % 10;

        u8g2.firstPage();
        do {
            // width 128 px
            //                              16px + 2px  minus
            // 21px + 2px  first digit           + 2px  padding
            // 21px + 4px  second digit
            //  5px + 4px  point
            // 21px + 2px  first point

            //  5px + 2px  *
            // 12px + 2px  C

            // Title
            u8g2.setDrawColor(1);
            u8g2.drawBox(0, 0, 128, 18);
            u8g2.setDrawColor(0);
            u8g2.setFont(u8g2_font_pxplusibmvga9_tr);
            u8g2.drawStr(4, 14, nowWaterState ? "WATER FULL" : "Discharge-TEMP");
            lastWaterState = nowWaterState;

            // Value
            u8g2.setDrawColor(1);
            u8g2.setFont(u8g2_font_logisoso42_tc);
            u8g2.setCursor(0, 63);
            u8g2.print(neg ? '-' : ' ');
            u8g2.setCursor(12 + (neg ? 8 : 0) + (digit2 == 1 ? 5: 0), 63);
            u8g2.print((char)(digit2 == 0 ? ' ': (digit2 + '0')));
            u8g2.setCursor(37 + (neg ? 8 : 0) + (digit1 == 1 ? 5: 0), 63);
            u8g2.print((char)(digit1 + '0'));
            u8g2.setCursor(62 + (neg ? 8 : 0), 63);
            u8g2.print('.');
            u8g2.setCursor(71 + (neg ? 8 : 0) + (point1 == 1 ? 5: 0), 63);
            u8g2.print(point1);

            // Unit
            u8g2.setFont(u8g2_font_logisoso24_tc);
            u8g2.setCursor(104, 63);
            u8g2.print((char)176);
            u8g2.print("C");
        } while (u8g2.nextPage());
    }
};

MINI_DISPLAY miniDisplay;

#endif