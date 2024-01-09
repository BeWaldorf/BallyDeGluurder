#ifndef WiFiManager_h
#define WiFiManager_h

#include <WiFi.h>

class WiFiManager
{
public:
    WiFiManager(const char *SSID, const char *PASSWORD, const int LED_PIN);
    void connect();
    bool isConnected();
    void loop();

private:
    const char *SSID;
    const char *PASSWORD;
    const int LED_PIN;
    unsigned long lastConnectAttemptMillis;
    static const unsigned long RECONNECT_INTERVAL = 30000;

    void reconnectIfNeeded();
};

#endif
