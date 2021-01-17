#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>

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
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  Blynk.virtualWrite(V1, currentTime);
}
void updateTemperature()
{
  temperatureSensor.requestTemperatures();
  float tempC = temperatureSensor.getTempCByIndex(0);
  String s = " -> " + String(tempC) + " 'C";
  Blynk.virtualWrite(V1, s);
}

StaticJsonDocument<100> blynkInputDoc;
DynamicJsonDocument projectDoc(500);
StaticJsonDocument<100> filterOption;

BLYNK_WRITE(V1)
{
  String inputString=param.asString();
  DeserializationError error = deserializeJson(blynkInputDoc, inputString);
  if (error) {
    Serial.print(F("deserializeJson() blynkInputDoc failed: "));
    Serial.println(error.f_str());
    return;
  }
   String inputDevice = blynkInputDoc["device"];
   inputDevice.toLowerCase();
   bool value = blynkInputDoc["value"];
   HTTPClient http;
   http.begin("http://blynk-cloud.com/9c2f660e4db14984a79a9c80076eb1d1/project");
   int httpCode = http.GET();
   String payload = http.getString();
   http.end();
   if(httpCode != 200) return;
   Serial.println(payload.length());
   blynkInputDoc.clear();
   filterOption ["widgets"] [0] ["label"] = true;
   filterOption ["widgets"] [0] ["pinType"] = true;
   filterOption ["widgets"] [0] ["pin"] = true;
   error = deserializeJson(projectDoc, payload,DeserializationOption::Filter(filterOption));
   filterOption.clear();
  if (error) {
    Serial.print(F("deserializeJson() projectDoc failed: "));
    Serial.println(error.f_str());
    return;
  }
  String output;
  serializeJson(projectDoc, output);
   Blynk.virtualWrite(V1,output);
  JsonArray widgets = projectDoc["widgets"].as<JsonArray>();
  projectDoc.clear();
  int virtualpinToBeUpdated=0;
  for(JsonVariant widget : widgets) 
  {
    if(!widget.containsKey("label")) continue; 
    String widgetDevice = widget["label"].as<String>();
    widgetDevice.toLowerCase();
    if (inputDevice == widgetDevice)
    {
      if (widget["pinType"] == "VIRTUAL")
        {
          virtualpinToBeUpdated=(widget["pin"].as<int>());
          Blynk.virtualWrite(virtualpinToBeUpdated,value);
          break;
        }
    }
  }
  delay(100);
  Blynk.syncVirtual(virtualpinToBeUpdated);
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
BLYNK_WRITE(V4)
{
  int state=param.asInt();
  state ? filter.SetOn() : filter.SetOff();
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  Blynk.syncAll();
  rtc.begin();
}