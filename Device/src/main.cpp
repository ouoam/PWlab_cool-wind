#include <Arduino.h>

#include "temp.h"
#include "display.h"
#include "io.h"

unsigned long nextPooling = 0;

void setup()
{
  temp.setup();
  display.setup();
  io.setup();
  io.setCool(true);
}

void loop()
{
  temp.loop();
  display.loop();

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
