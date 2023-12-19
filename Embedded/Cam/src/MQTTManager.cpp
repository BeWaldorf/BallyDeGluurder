#include "MQTTManager.h"

MQTTManager::MQTTManager(Client &client, const char *server, int port, const char *user, const char *password, const char *clientId)
    : mqttClient(client)
{
    this->server = server;
    this->port = port;
    this->user = user;
    this->password = password;
    this->clientId = clientId;
}

void MQTTManager::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    mqttClient.setCallback(callback);
}

void MQTTManager::connect()
{
    Serial.print("Connecting to MQTT ..");
    mqttClient.setServer(server, port);
    while (!mqttClient.connected())
    {
        Serial.print(".");
        if (mqttClient.connect(clientId, user, password))
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

bool MQTTManager::isConnected()
{
    return mqttClient.connected();
}

void MQTTManager::loop()
{
    mqttClient.loop();
}

void MQTTManager::subscribe(const char *topic)
{
    mqttClient.subscribe(topic);
}

void MQTTManager::publish(const char *topic, const char *payload)
{
    mqttClient.publish(topic, payload);
}