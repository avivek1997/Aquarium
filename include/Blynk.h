#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "9c2f660e4db14984a79a9c80076eb1d1";
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(D4);
DallasTemperature temperatureSensor(&oneWire);
BlynkTimer timer;

WidgetRTC rtc;

void updateBlynk()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  // Serial.print("Current time: ");
  // Serial.print(currentTime);
  // Serial.print(" ");
  // Serial.print(currentDate);
  // Serial.println();
  // Send time to the App
  Blynk.virtualWrite(V2, light.currentState);
  Blynk.virtualWrite(V3, outdoorLight.currentState);
  
  // Send date to the App
  //Blynk.virtualWrite(V2, currentDate);
}
void updateTemperature()
{
  temperatureSensor.requestTemperatures();
  float tempC = temperatureSensor.getTempCByIndex(0);
  String s += " -> " + String(tempC) + " 'C";
  Blynk.virtualWrite(V1, s);
}
BLYNK_WRITE(V2)
{
  int state=param.asInt();
  state ? light.SetOn() : light.SetOff();
}
BLYNK_WRITE(V3)
{
  int state=param.asInt();
  state ? outdoorLight.SetOn() : outdoorLight.SetOff();
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}