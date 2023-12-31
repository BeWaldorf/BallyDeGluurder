#ifndef MQTTManager_h
#define MQTTManager_h

#include <PubSubClient.h>

class MQTTManager
{
public:
    MQTTManager(Client &client, const char *server, int port, const char *user, const char *password, const char *clientId);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void connect();
    bool isConnected();
    void loop();
    void subscribe(const char *topic);
    void publish(const char *topic, const char *payload);
    void printMessage(char *topic, byte *payload, unsigned int length);

private:
    PubSubClient mqttClient;
    const char *server;
    int port;
    const char *user;
    const char *password;
    const char *clientId;
};

#endif
