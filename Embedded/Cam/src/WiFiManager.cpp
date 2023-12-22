#include "WiFiManager.h"

WiFiManager::WiFiManager(const char *SSID, const char *PASSWORD, const int LED_PIN)
    : SSID(SSID), PASSWORD(PASSWORD), LED_PIN(LED_PIN)
{
}

void WiFiManager::connect()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.print("Connecting to WiFi ..");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::loop()
{
    unsigned long currentMillis = millis();

    Serial.println(WiFi.RSSI());

    if (isConnected())
    {
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }

    if (!isConnected() && currentMillis - lastConnectAttemptMillis >= RECONNECT_INTERVAL)
    {
        reconnectIfNeeded();
    }
}

void WiFiManager::reconnectIfNeeded()
{
    Serial.println("Reconnecting WiFi");
    WiFi.disconnect();
    WiFi.reconnect();
    lastConnectAttemptMillis = millis();
}