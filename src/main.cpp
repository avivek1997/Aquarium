#include <ESP8266WiFi.h>
#include <OTA.h>
#include <OutputDevices.h>
#include <ESPServo.h>
#include <ArduinoJson.h>

#define Light_Pin D7
#define Filter_Pin D8
#define Relay1_Pin D0
#define Relay2_Pin 10

OutputDevice light(Light_Pin, "Light", 0);
OutputDevice outdoorLight(Relay1_Pin, "Out Door Light", 0);
OutputDevice filter(Filter_Pin, "Filter", 0);
const char *ssid = "Vivek1";
const char *password = "vivek2271146";
#include <Blynk.h>

BLYNK_WRITE(V5)
{
  myServo.write(param.asInt());
  Blynk.virtualWrite(V1, param.asInt());
}

void HandleLights(bool isFromLoop)
{
  if (!isFromLoop)
  {
    if (hour() >= 18 && hour() < 21)
      light.SetOn();
    else
      light.SetOff();
    if (hour() >= 18 && hour() < 22)
      outdoorLight.SetOn();
    else
      outdoorLight.SetOff();
  }
  else
  {
    if (hour() == 18 && minute() == 0)
      light.SetOn();
    else if (hour() == 21 && minute() == 0)
      light.SetOff();
    if (hour() == 18 && minute() == 0)
      outdoorLight.SetOn();
    else if (hour() == 22 && minute() == 0)
      outdoorLight.SetOff();
  }
}
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  temperatureSensor.begin();
  filter.SetOn();  
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Wifi Connection Failed! Rebooting...");
    delay(5000);
    setup();
  }
  InitializeOTA();
  Blynk.begin(auth, ssid, password);
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  timer.setInterval(5000L, updateBlynk);
  timer.setInterval(5000L, updateTemperature);
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //HandleLights(false);
}

void loop()
{
  ArduinoOTA.handle();
  Blynk.run();
  timer.run();
  //HandleLights(true);
}

