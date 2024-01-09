#include "MotorDriver.h"
#include <Arduino.h>

MotorDriver::MotorDriver(const int EN1, const int IN1, const int IN2, const int IN3, const int IN4, const int EN2)
    : EN1(EN1), IN1(IN1), IN2(IN2), IN3(IN3), IN4(IN4), EN2(EN2)
{
}

void MotorDriver::setup()
{
    pinMode(EN1, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(EN2, OUTPUT);
}

void MotorDriver::forward(int speed)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN1, speed);
    analogWrite(EN2, speed);
}

void MotorDriver::backward(int speed)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(EN1, speed);
    analogWrite(EN2, speed);
}

void MotorDriver::left(int speed)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN1, speed);
    analogWrite(EN2, speed);
}

void MotorDriver::right(int speed)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(EN1, speed);
    analogWrite(EN2, speed);
}

void MotorDriver::stop()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(EN1, 0);
    analogWrite(EN2, 0);
}

void MotorDriver::drive(char direction, int speed)
{
    if (direction == 'f')
    {
        forward(speed);
    }
    else if (direction == 'b')
    {
        backward(speed);
    }
    else if (direction == 'l')
    {
        left(speed);
    }
    else if (direction == 'r')
    {
        right(speed);
    }
    else if (direction == 's')
    {
        stop();
    }
}