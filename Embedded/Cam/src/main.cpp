#include <WiFi.h>
#include "esp_camera.h"
#include <ArduinoWebsockets.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WebServer.h>

using namespace websockets;

WebServer server(80);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char *websockets_server = "ws://23.97.138.160:3000";

const char *WIFI_SSID = "BillyTheRobot";
const char *WIFI_PASSWORD = "eloict1234";

const char *MQTT_SERVER = "23.97.138.160";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "bally";
const char *MQTT_PASSWORD = "BallyDeGluurder";
const String MQTT_CLIENTID = "ESP3-" + String(random(0xffff), HEX);

const int FLASH_PIN = 4;

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const int LED_PIN = 33;

unsigned long startMillis = 0;

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

  int speed = doc["flash"].as<int>();

  Serial.println("Speed: " + String(speed));

  delay(15);

  digitalWrite(FLASH_PIN, speed);
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

WebsocketsClient client;

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

void handle_jpg_stream(void)
{
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  server.sendContent(response);

  while (1)
  {
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb)
    {
      Serial.println("Camera capture failed");
      return;
    }
    response = "--frame\r\n";
    response += "Content-Type: image/jpeg\r\n\r\n";
    server.sendContent(response);
    client.write((char *)fb->buf, fb->len);
    server.sendContent("\r\n");
    esp_camera_fb_return(fb);

    if (!client.connected())
    {
      break;
    }
  }
}

void startCameraServer()
{
  server.on("/stream", HTTP_GET, handle_jpg_stream);
  server.begin();
  Serial.println("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  // Camera configuratie
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  connectToWifi();
  connectToMqtt();
  mqttClient.subscribe("bally/flash");

  // Setup Callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  // Connect to server
  client.connect(websockets_server);

  startCameraServer();
}

void sendPic()
{
  client.poll();
  if (WiFi.status() == WL_CONNECTED)
  {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
      Serial.println("Camera capture failed");
      return;
    }

    // Verstuur het frame als een binair bericht
    client.sendBinary((const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
  }
  delay(5000);
}

void loop()
{
  // unsigned long currentMillis = millis();

  // wifiWatchdog(currentMillis);

  // if (WiFi.status() == WL_CONNECTED)
  // {
  //   digitalWrite(LED_PIN, LOW);
  //   sendPic();

  //   if (!mqttClient.connected())
  //   {
  //     connectToMqtt();
  //   }

  //   mqttClient.loop();
  // }
}