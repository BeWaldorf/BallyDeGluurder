#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>

#include "WiFiManager.h"
#include "MQTTManager.h"
#include "CameraManager.h"

using namespace websockets;

const char *WIFI_SSID = "BillyTheRobot1";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP32-" + String(random(0xffff), HEX);

const char *WEBSOCKET_HOST = "23.97.138.160";
const uint16_t WEBSOCKET_PORT = 3000;

const int LED_PIN = 33;
const int FLASH_PIN = 4;

WiFiClient wifiClient;
WebsocketsClient websocketClient;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, LED_PIN);
MQTTManager mqttManager(wifiClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENTID.c_str());
CameraManager cameraManager(FLASH_PIN);

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  mqttManager.printMessage(topic, payload, length);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  int value = doc["flash"].as<int>();

  cameraManager.setFlash(value);
}

void onMessageCallback(WebsocketsMessage message)
{
  Serial.print("Got Message: ");
  Serial.println(message.data());
}

void setup()
{
  Serial.begin(115200);

  wifiManager.connect();

  mqttManager.setCallback(callbackMQTT);
  mqttManager.connect();
  mqttManager.subscribe("bally/flash");

  websocketClient.onMessage(onMessageCallback);
  websocketClient.connect(WEBSOCKET_HOST, WEBSOCKET_PORT, "/");

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

    camera_fb_t *cameraFrameBuffer = esp_camera_fb_get();
    if (!cameraFrameBuffer)
    {
      Serial.println("Camera capture failed");
      return;
    }

    websocketClient.sendBinary((const char *)cameraFrameBuffer->buf, cameraFrameBuffer->len);
    esp_camera_fb_return(cameraFrameBuffer);

    delay(100);
  }
}
