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
const float R2 = 1000000.0; // 1MΩ
const float R1 = 560000.0;  // 560KΩ
const float VOLTAGE_DIVIDER = (R1 + R2) / R2;
const float BATTERY_MAX_VOLTAGE = 4.8;
const float BATTERY_MIN_VOLTAGE = 3.6;
const float ADC_RESOLUTION = 4095.0;

const float MINUTES_DRIVING_PER_PERCENTAGE = 2.1;

const int MOTOR1_ENA = 21;
const int MOTOR1_IN1 = 5;
const int MOTOR1_IN2 = 23;

const int MOTOR2_IN3 = 12;
const int MOTOR2_IN4 = 13;
const int MOTOR2_ENB = 14;

const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;
const int MOTOR_LEFT_PWM_CHANNEL = 0;
const int MOTOR_RIGHT_PWM_CHANNEL = 1;

char direction;
int speed;

unsigned long startupTime = millis();

unsigned long startMillis = 0;
unsigned long lastMessageMillis = 0;

unsigned long lastBatteryPublishMillis = 0;
const long batteryPublishInterval = 10000;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD, LED_PIN);
MQTTManager mqttManager(espClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENTID.c_str());
MotorDriver motorDriver(MOTOR1_ENA, MOTOR1_IN1, MOTOR1_IN2, MOTOR2_IN3, MOTOR2_IN4, MOTOR2_ENB);

float getBatteryVoltage()
{
  int adcValue = analogRead(BATTERY_PIN);
  float batteryVoltage = (adcValue / ADC_RESOLUTION) * 3.3 * VOLTAGE_DIVIDER;
  return batteryVoltage;
}

int getBatteryPercentage(float batteryVoltage)
{
  int batteryPercentage = (batteryVoltage - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) * 100;
  batteryPercentage = constrain(batteryPercentage, 0, 100);
  return batteryPercentage;
}

int getUpTime()
{
  unsigned long currentTime = millis();
  unsigned long runningTime = (currentTime - startupTime) / 1000;
  return runningTime;
}

int getTimeLeft(int batteryPercentage)
{
  return batteryPercentage * MINUTES_DRIVING_PER_PERCENTAGE;
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

  motorDriver.setup();
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

    float batteryVoltage = getBatteryVoltage();
    int batteryPercentage = getBatteryPercentage(batteryVoltage);
    int upTime = getUpTime();
    int timeLeft = getTimeLeft(batteryPercentage);

    unsigned long currentMillis = millis();
    if (currentMillis - lastBatteryPublishMillis >= batteryPublishInterval)
    {
      DynamicJsonDocument doc(1024);
      doc["voltage"] = batteryVoltage;
      doc["percentage"] = batteryPercentage;
      doc["uptime"] = upTime;
      doc["timeleft"] = timeLeft;

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
    drawBatteryIndicator(88, map(batteryPercentage, 0, 100, 1, 5), "Bat.");
    showDisplay();
  }
}
