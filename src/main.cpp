#include <ESP8266WiFi.h>
#include <OTA.h>
#include <Blynk.h>
#include <OutputDevices.h>
#include <ESPServo.h>
#define Light_Pin D7
#define Heater_Pin D8
#define Relay1_Pin D0
#define Relay2_Pin 10
OutputDevice light(Light_Pin,"Light",0);
OutputDevice outdoorLight(Relay1_Pin,"Out Door Light",0);
const char* ssid = "Vivek1";
const char* password = "vivek2271146";
BLYNK_WRITE(V3)
{
  myServo.write(param.asInt());
  Blynk.virtualWrite(V1, param.asInt());
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Wifi Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  InitializeOTA();
  Blynk.begin(auth, ssid, password);
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  timer.setInterval(10000L, clockDisplay);
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  Blynk.run();
  timer.run();
  if(hour()>=18 && hour()<21) light.SetOn();
  else light.SetOff();
  if(hour()>=18 && hour()<22) outdoorLight.SetOn();
  else outdoorLight.SetOff();
}