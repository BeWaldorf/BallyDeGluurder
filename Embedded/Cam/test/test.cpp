#include <Arduino.h>
#include <WiFi.h>
#include <esp_camera.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

// Vervang met je WiFi credentials
const char *ssid = "JoKeWi";
const char *password = "Titap;4JkW.be";

WebsocketsClient webSocket;

// Vervang met het WebSocket adres van je Node.js server
const char *websockets_server_host = "23.97.138.160";
const uint16_t websockets_server_port = 3000; // of een andere poort

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

    // WiFi verbinding
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi verbonden");

    // WebSocket setup
    webSocket.onMessage(onMessageCallback);
    webSocket.onEvent(onEventsCallback);
    webSocket.connect(websockets_server_host, websockets_server_port, "/");
}

void loop()
{
    webSocket.poll();

    if (WiFi.status() == WL_CONNECTED)
    {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera capture failed");
            return;
        }

        // Verstuur het frame als een binair bericht
        webSocket.sendBinary((const char *)fb->buf, fb->len);
        esp_camera_fb_return(fb);
    }

    delay(5000); // Verstuur elke 5 seconden een frame (aanpasbaar)
}
