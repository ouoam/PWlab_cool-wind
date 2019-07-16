#include <Arduino.h>

/*

Pin Name    Function    Connect
2   D0  PD0 Rx          Nextion HMI Tx
3   D1  PD1 Tx          NEXtion HMI Rx
4   D2  PD2             DS18B20 Pre-temp
5   D3  PD3 INT0  PWM   DS18B20 Ex-temp
6   D4  PD4 INT1        Water Full Sensor
11  D5  PD5       PWM   
12  D6  PD6       PWM   
13  D7  PD7             
14  D8  PB0             HC138 A
15  D9  PB1       PWM   HC138 B
16  D10 PB2 SS    PWM   HC138 C
17  D11 PB3 MOSI  PWM   Compressor
18  D12 PB4 MISO        Blower
19  D13 PB5 SCK         Buzzer
23  A0  PC0             
24  A1  PC1             
25  A2  PC2             
26  A3  PC3             
27  A4  PC4 SDA         OLED SDA
28  A5  PC5 SCL         OLED SCL


                _______
                |     |
  8 22 GND ---- | * * | ---> RST   1
  17  MOSI <--- | * *   ---> SCK   19
  7 20 VCC ---- | * * | <--- MISO  18
                |_____|

 */

#include "temp.h"
#include "display.h"
#include "io.h"
#include "miniDisplay.h"

unsigned long nextPooling = 0;

void setup()
{
  temp.setup();
  display.setup();
  io.setup();
  miniDisplay.setup();
  io.setCool(true);
}

void loop()
{
  temp.loop();
  display.loop();
  miniDisplay.loop();

  if (nextPooling <= millis())
  {
    if (page == 0) {
      int temppp = display.getPreTemp();

      display.setETAtime(temp.getEstimateTimeTo(temppp) / 1000);
      if (temppp >= temp.getLastTemp())
      {
        display.setStatus(2);
        display.setButton(B1);
      }
      else
      {
        if (display.getStatus() != 0 && display.getStatus() != 1)
          display.setStatus(1);
        display.setButton(B0);
      }
    }

    nextPooling = millis() + 250;
  }
}
