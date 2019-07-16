#ifndef MINI_DISPLAY_h
#define MINI_DISPLAY_h

#include <Arduino.h>

#include <U8x8lib.h>
#include <Wire.h>

#include "temp.h"

class MINI_DISPLAY
{
private:
    U8X8_SH1106_128X64_NONAME_HW_I2C u8x8 = U8X8_SH1106_128X64_NONAME_HW_I2C(/* reset=*/ U8X8_PIN_NONE);
    float oldTemp = -128;
public:
    void setup()
    {
        u8x8.begin();
        //u8x8.setFlipMode(1);

        u8x8.setFont(u8x8_font_px437wyse700b_2x2_r);    
        u8x8.clear();

        u8x8.inverse();
        u8x8.print("Ex  Temp");
        
        u8x8.noInverse();
        u8x8.setFont(u8x8_font_inb33_3x6_n);
        // u8x8.setCursor(9, 2);
        // u8x8.print('*');
        // u8x8.print('C');
    }

    void loop()
    {
        float tempC = temp.getLastExTemp();
        if (oldTemp == tempC) return;
        
        u8x8.setCursor(0, 2);
        u8x8.print(tempC,1);
    }
};

MINI_DISPLAY miniDisplay;

#endif