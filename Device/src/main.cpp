#include <Arduino.h>

#include "temp.h"
#include "display.h"
#include "io.h"

Temp temp;
Display display;
IO io;

uint8_t time = 10;

void setup()
{
  temp.setup();
  display.setup();
  io.setup();
  io.setCool(true);
}

void loop()
{
  String out = "";
  out += "Temp:";
  out += temp.getLastTemp();

  int temppp = display.getPreTemp();

  temp.loop();
  display.loop();
  display.setETAtime(temp.getEstimateTimeTo(temppp) / 1000);
  if (temppp >= temp.getLastTemp())
  {
    out += " +";
    out += temppp;
    display.setStatus(2);
    display.setButton(B1);
  }
  else
  {
    out += " -";
    out += temppp;
    if (display.getStatus() != 0 && display.getStatus() != 1)
      display.setStatus(1);
    display.setButton(B0);
  }

  display.setDebug(out);
}
