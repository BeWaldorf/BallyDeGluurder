#include <WiFi.h>
#include "esp_camera.h"
#include <HTTPClient.h>

WiFiClient espClient;

const char *WIFI_SSID = "JoKeWi";
const char *WIFI_PASSWORD = "Titap;4JkW.be";

const char *SERVER_NAME = "http://13.93.84.122:3000/upload";

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const int LED_PIN = 33;

unsigned long startMillis = 0;

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
}

void sendPic()
{
  camera_fb_t *fb = NULL;

  // Neem een foto
  fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    return;
  }

  // Verstuur de foto
  HTTPClient http;
  http.begin(SERVER_NAME);
  http.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = http.POST(fb->buf, fb->len);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  }
  else
  {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  // Bevrijd de geheugen
  esp_camera_fb_return(fb);

  // Wacht een tijdje voor de volgende foto
  delay(5000);
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