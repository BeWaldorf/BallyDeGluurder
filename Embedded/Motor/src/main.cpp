#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "display.h"
#include "MotorDriver.h"

WiFiClient espClient;

const char *WIFI_SSID = "BillyTheRobot1";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP32-" + String(random(0xffff), HEX);

const int LED_PIN = 2;
const int BATTERY_PIN = 34;
const float R1 = 1000000; // 1MΩ
const float R2 = 560000;  // 560KΩ
const float BATTERY_MAX_VOLTAGE = 4.8;

const int MOTOR1_ENA = 21;
const int MOTOR1_IN1 = 22;
const int MOTOR1_IN2 = 23;

const int MOTOR2_IN3 = 12;
const int MOTOR2_IN4 = 13;
const int MOTOR2_ENB = 14;

const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;
const int MOTOR_LEFT_PWM_CHANNEL = 0;
const int MOTOR_RIGHT_PWM_CHANNEL = 1;

bool newMQTTMessage = false;
char direction = 's'; // Standaardwaarde, bijvoorbeeld 's' voor stop
int speed = 0;

unsigned long startMillis = 0;
unsigned long lastMessageMillis = 0;

unsigned long lastBatteryPublishMillis = 0;
const long batteryPublishInterval = 1000;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, LED_PIN);
MQTTManager mqttManager(espClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENTID.c_str());
MotorDriver motorDriver(MOTOR1_ENA, MOTOR1_IN1, MOTOR1_IN2, MOTOR2_IN3, MOTOR2_IN4, MOTOR2_ENB);

int readBatteryLevel()
{
  int analogValue = analogRead(BATTERY_PIN);
  float voltage = (analogValue / 4095.0) * 3.3;
  float batteryVoltage = voltage / (R2 / (R1 + R2));
  float level = map(batteryVoltage, 0, BATTERY_MAX_VOLTAGE, 1, 5);
  return level;
}

void updateMotorControl(char direction, int speed)
{
  motorDriver.drive(direction, speed);

  int leftSpeed = 0;
  int rightSpeed = 0;

  if (direction == 'f')
  {
    leftSpeed = speed;
    rightSpeed = speed;
  }
  else if (direction == 'b')
  {
    leftSpeed = -speed;
    rightSpeed = -speed;
  }
  else if (direction == 'l')
  {
    leftSpeed = -speed;
    rightSpeed = speed;
  }
  else if (direction == 'r')
  {
    leftSpeed = speed;
    rightSpeed = -speed;
  }
  else if (direction == 's')
  {
    leftSpeed = 0;
    rightSpeed = 0;
  }

  drawVerticalSlider(0, leftSpeed, "Links");
  drawVerticalSlider(44, rightSpeed, "Rechts");
}

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  newMQTTMessage = true;
  mqttManager.printMessage(topic, payload, length);

  if (strcmp(topic, "bally/directions") == 0)
  {
    Serial.println("Parsing JSON" + String((char *)payload));

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    direction = doc["direction"].as<String>().charAt(0);
    speed = doc["speed"].as<int>();

    clearDisplay();
    updateMotorControl(direction, speed); // Deze functie stuurt de motoren aan en tekent de sliders
    showDisplay();
  }
}

void setup()
{
  Serial.begin(115200);
  initDisplay();

  wifiManager.connect();
  mqttManager.setCallback(callbackMQTT);
  mqttManager.connect();
  mqttManager.subscribe("bally/directions");
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

    int batteryLevel = readBatteryLevel();

    unsigned long currentMillis = millis();
    if (currentMillis - lastBatteryPublishMillis >= batteryPublishInterval)
    {
      DynamicJsonDocument doc(1024);
      doc["battery"] = batteryLevel;

      // Serialize JSON object to String
      String payload;
      serializeJson(doc, payload);

      // Publish battery level to MQTT topic
      mqttManager.publish("bally/battery", payload.c_str());

      // Update the last publish time
      lastBatteryPublishMillis = currentMillis;
    }

    clearDisplay();
    updateMotorControl(direction, speed);
    drawBatteryIndicator(88, batteryLevel, "Bat.");
    showDisplay();
  }
}
