#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(2); // PORT For ds18b20
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temp C: ");
  Serial.println(tempC, 6);
}
