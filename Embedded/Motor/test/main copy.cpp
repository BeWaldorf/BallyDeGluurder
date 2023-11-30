#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char *WIFI_SSID = "BillyTheRobot";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP32-" + String(random(0xffff), HEX);

const int LED_PIN = 2;

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

unsigned long startMillis = 0;
unsigned long lastMessageMillis = 0;

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

  Serial.println("Parsing JSON" + String((char *)payload));

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  char direction = doc["direction"].as<String>().charAt(0);
  int speed = doc["speed"].as<int>();

  Serial.print("Direction: ");
  Serial.println(direction);
  Serial.print("Speed: ");
  Serial.println(speed);

  ledcWrite(MOTOR_LEFT_PWM_CHANNEL, speed);
  delay(15);

  if (direction == 'f')
  {
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, LOW);
    digitalWrite(MOTOR2_IN3, HIGH);
    digitalWrite(MOTOR2_IN4, LOW);
  }
  else if (direction == 'b')
  {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, HIGH);
    digitalWrite(MOTOR2_IN3, LOW);
    digitalWrite(MOTOR2_IN4, HIGH);
  }
  else if (direction == 'l')
  {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, HIGH);
    digitalWrite(MOTOR2_IN3, HIGH);
    digitalWrite(MOTOR2_IN4, LOW);
  }
  else if (direction == 'r')
  {
    digitalWrite(MOTOR1_IN1, HIGH);
    digitalWrite(MOTOR1_IN2, LOW);
    digitalWrite(MOTOR2_IN3, LOW);
    digitalWrite(MOTOR2_IN4, HIGH);
  }
  else if (direction == 's')
  {
    digitalWrite(MOTOR1_IN1, LOW);
    digitalWrite(MOTOR1_IN2, LOW);
    digitalWrite(MOTOR2_IN3, LOW);
    digitalWrite(MOTOR2_IN4, LOW);
  }
}

void connectToWifi()
{
  Serial.print("Connecting to WiFi ..");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void connectToMqtt()
{
  Serial.print("Connecting to MQTT ..");
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callbackMQTT);
  while (!mqttClient.connected())
  {
    Serial.print(".");
    if (mqttClient.connect(MQTT_CLIENTID.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN3, OUTPUT);
  pinMode(MOTOR2_IN4, OUTPUT);
  pinMode(MOTOR1_ENA, OUTPUT);
  pinMode(MOTOR2_ENB, OUTPUT);

  ledcSetup(MOTOR_LEFT_PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR1_ENA, MOTOR_LEFT_PWM_CHANNEL);

  ledcSetup(MOTOR_RIGHT_PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(MOTOR2_ENB, MOTOR_RIGHT_PWM_CHANNEL);

  connectToWifi();
  connectToMqtt();
  mqttClient.subscribe("bally/directions");
}

void wifiWatchdog(unsigned long currentMillis)
{
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - startMillis >= 30000))
  {
    Serial.println("Reconnecting WiFi");
    digitalWrite(LED_PIN, LOW);
    WiFi.disconnect();
    WiFi.reconnect();
    startMillis = currentMillis;
  }
}

void loop()
{
  unsigned long currentMillis = millis();

  wifiWatchdog(currentMillis);

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_PIN, HIGH);

    if (!mqttClient.connected())
    {
      connectToMqtt();
    }

    mqttClient.loop();

    // // hier periodiek publishen
    // if (currentMillis - lastMessageMillis > 5000)
    // {
    //   lastMessageMillis = millis();
    //   counter++;
    //   String msg = "hello world " + String(counter);
    //   String topic = "ucll/test";
    //   mqttClient.publish(topic.c_str(), msg.c_str());
    //   Serial.print("Boodschap gepubliceerd voor topic [" + topic + "] : ");
    //   Serial.println(msg);
    // }
  }
}