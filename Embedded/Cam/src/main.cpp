#include <WiFi.h>
#include "esp_camera.h"
#include <ArduinoWebsockets.h>

using namespace websockets;

WiFiClient espClient;

const char *websockets_server = "ws://23.97.138.160:3000";

const char *WIFI_SSID = "JoKeWi";
const char *WIFI_PASSWORD = "Titap;4JkW.be";

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const int LED_PIN = 33;

unsigned long startMillis = 0;

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  connectToWifi();

  // Setup Callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  // Connect to server
  client.connect(websockets_server);

  // Send a message
  client.send("Hi Server!");
  // Send a ping
  client.ping();
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
  // delay(100);
}

void loop()
{
  unsigned long currentMillis = millis();

  wifiWatchdog(currentMillis);

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_PIN, LOW);
    sendPic();
  }
}