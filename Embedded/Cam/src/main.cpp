#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "CameraManager.h"

WiFiClient espClient;

const char *WIFI_SSID = "BillyTheRobot1";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP32-" + String(random(0xffff), HEX);

const int LED_PIN = 33;
const int FLASH_PIN = 4;

const int PWM_FREQUENCY = 20000;
const int PWM_LED_CHANNEL = 0;
const int PWN_RESOLUTION = 8;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, LED_PIN);
MQTTManager mqttManager(espClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENTID.c_str());
CameraManager cameraManager(FLASH_PIN, PWM_FREQUENCY, PWM_LED_CHANNEL, PWN_RESOLUTION);

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  mqttManager.printMessage(topic, payload, length);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  int value = doc["flash"].as<int>();

  cameraManager.setFlash(value);
}

void setup()
{
  Serial.begin(115200);

  wifiManager.connect();

  mqttManager.setCallback(callbackMQTT);
  mqttManager.connect();
  mqttManager.subscribe("bally/flash");

  cameraManager.initialize();
}

void loop()
{
  wifiManager.loop();
  if (wifiManager.isConnected())
  {
    if (!mqttManager.isConnected())
    {
      mqttManager.connect();
    }
    mqttManager.loop();
  }
}
