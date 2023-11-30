#include "WiFiManager.h"

WiFiManager::WiFiManager(const char *ssid, const char *password)
{
    this->ssid = ssid;
    this->password = password;
}

void WiFiManager::connect()
{
    Serial.print("Connecting to WiFi ..");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
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

void WiFiManager::reconnectIfNeeded()
{
    if (!isConnected())
    {
        connect();
    }
}
