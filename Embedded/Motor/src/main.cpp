#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "display.h"

WiFiClient espClient;

const char *WIFI_SSID = "BillyTheRobot";
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

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
MQTTManager mqttManager(espClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENTID.c_str());

int readBatteryLevel()
{
  int analogValue = analogRead(BATTERY_PIN);
  float voltage = (analogValue / 4095.0) * 3.3;
  float batteryVoltage = voltage / (R2 / (R1 + R2));
  float level = map(batteryVoltage, 0, BATTERY_MAX_VOLTAGE, 1, 5);
  return level;
}

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

void updateMotorControl(char direction, int speed)
{
  ledcWrite(MOTOR_LEFT_PWM_CHANNEL, speed);
  delay(15);

  if (direction == 'f')
  {
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, LOW);
    digitalWrite(MOTOR2_IN3, HIGH);
    digitalWrite(MOTOR2_IN4, LOW);
    drawVerticalSlider(0, speed, "Links");
    drawVerticalSlider(44, speed, "Rechts");
  }
  else if (direction == 'b')
  {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, HIGH);
    digitalWrite(MOTOR2_IN3, LOW);
    digitalWrite(MOTOR2_IN4, HIGH);
    drawVerticalSlider(0, -speed, "Links");
    drawVerticalSlider(44, -speed, "Rechts");
  }
  else if (direction == 'l')
  {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, HIGH);
    digitalWrite(MOTOR2_IN3, HIGH);
    digitalWrite(MOTOR2_IN4, LOW);
    drawVerticalSlider(0, speed, "Links");
    drawVerticalSlider(44, 0, "Rechts");
  }
  else if (direction == 'r')
  {
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, LOW);
    digitalWrite(MOTOR2_IN3, LOW);
    digitalWrite(MOTOR2_IN4, HIGH);
    drawVerticalSlider(0, 0, "Links");
    drawVerticalSlider(44, speed, "Rechts");
  }
  else if (direction == 's')
  {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, LOW);
    digitalWrite(MOTOR2_IN3, LOW);
    digitalWrite(MOTOR2_IN4, LOW);
    drawVerticalSlider(0, 0, "Links");
    drawVerticalSlider(44, 0, "Rechts");
  }
}

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  newMQTTMessage = true;
  printMqttMessage(topic, payload, length);

  Serial.println("Parsing JSON" + String((char *)payload));

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  direction = doc["direction"].as<String>().charAt(0);
  speed = doc["speed"].as<int>();

  Serial.print("Direction: ");
  Serial.println(direction);
  Serial.print("Speed: ");
  Serial.println(speed);

  clearDisplay();
  updateMotorControl(direction, speed); // Deze functie stuurt de motoren aan en tekent de sliders
  showDisplay();
}

void setup()
{
  Serial.begin(115200);
  initDisplay();

  wifiManager.connect();
  mqttManager.setCallback(callbackMQTT);
  mqttManager.connect();
  mqttManager.subscribe("bally/joystick");
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

    int batteryLevel = readBatteryLevel();

    clearDisplay();
    updateMotorControl(direction, speed);
    drawBatteryIndicator(88, batteryLevel, "Bat.");
    showDisplay();
  }
}
