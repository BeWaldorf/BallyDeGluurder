#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "MQTTManager.h"

WiFiClient espClient;

const char *WIFI_SSID = "BillyTheRobot";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP32-" + String(random(0xffff), HEX);

const int FLASH_PIN = 4;

const int freq = 20000;
const int ledChannel = 0;
const int resolution = 8;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
MQTTManager mqttManager(espClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENTID.c_str());

void printMqttMessage(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Boodschap ontvangen voor topic [");
  Serial.print(topic);
  Serial.print("] : ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  printMqttMessage(topic, payload, length);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  int value = doc["flash"].as<int>();

  ledcWrite(ledChannel, value);
}

void setup()
{
  Serial.begin(115200);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(FLASH_PIN, ledChannel);

  wifiManager.connect();
  mqttManager.setCallback(callbackMQTT);
  mqttManager.connect();
  mqttManager.subscribe("bally/flash");
}

void loop()
{
  wifiManager.reconnectIfNeeded();
  if (wifiManager.isConnected())
  {
    if (!mqttManager.isConnected())
    {
      mqttManager.connect();
    }
    mqttManager.loop();
  }
}
