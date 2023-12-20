#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "CameraManager.h"
#include <ArduinoWebsockets.h>

using namespace websockets;

WiFiClient espClient;
WebsocketsClient webSocket;

const char *WIFI_SSID = "BillyTheRobot1";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP32-" + String(random(0xffff), HEX);

const char *WEBSOCKET_HOST = "23.97.138.160";
const uint16_t WEBSOCKET_PORT = 3000; // of een andere poort

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

void onMessageCallback(WebsocketsMessage message)
{
  Serial.print("Got Message: ");
  Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data)
{
  if (event == WebsocketsEvent::ConnectionOpened)
  {
    Serial.println("Connnection Opened");
  }
  else if (event == WebsocketsEvent::ConnectionClosed)
  {
    Serial.println("Connnection Closed");
  }
  else if (event == WebsocketsEvent::GotPing)
  {
    Serial.println("Got a Ping!");
  }
  else if (event == WebsocketsEvent::GotPong)
  {
    Serial.println("Got a Pong!");
  }
}

void setup()
{
  Serial.begin(115200);

  wifiManager.connect();

  mqttManager.setCallback(callbackMQTT);
  mqttManager.connect();
  mqttManager.subscribe("bally/flash");

  webSocket.onMessage(onMessageCallback);
  webSocket.onEvent(onEventsCallback);
  webSocket.connect(WEBSOCKET_HOST, WEBSOCKET_PORT, "/");

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

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
      Serial.println("Camera capture failed");
      return;
    }

    // Verstuur het frame als een binair bericht
    webSocket.sendBinary((const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);

    delay(100);
  }
}
