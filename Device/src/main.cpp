#include <Arduino.h>

#include "temp.h"
#include "display.h"
#include "io.h"

Temp temp;
Display display;
IO io;

uint8_t time = 10;
int8_t pretemp = 9;

void setup()
{
  // Serial.begin(115200);
  temp.setup();
  display.setup();
  io.setup();

  io.setCool(true);
}

void loop()
{
  temp.loop();
  display.loop();
  display.setTemp(temp.getLastTemp());
  display.setETAtime(temp.getEstimateTimeTo(pretemp) / 1000);
  // Serial.println(display.printStatus());
}
