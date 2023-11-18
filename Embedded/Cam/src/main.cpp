#include <Arduino.h>

#define LED_PIN 4 // Definieer de LED-pin (GPIO 4)

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Zet de LED-pin als uitgang
}

void loop()
{
  digitalWrite(LED_PIN, LOW);  // Zet de LED aan
  delay(1000);                 // Wacht 1 seconde
  digitalWrite(LED_PIN, HIGH); // Zet de LED uit
  delay(1000);                 // Wacht 1 seconde
}