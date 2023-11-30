#ifndef WiFiManager_h
#define WiFiManager_h

#include <WiFi.h>

class WiFiManager
{
public:
    WiFiManager(const char *ssid, const char *password);
    void connect();
    bool isConnected();
    void reconnectIfNeeded();

private:
    const char *ssid;
    const char *password;
};

#endif
