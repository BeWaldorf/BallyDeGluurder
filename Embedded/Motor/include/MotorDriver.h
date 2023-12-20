#ifndef MotorDriver_h
#define MotorDriver_h

class MotorDriver
{
public:
    MotorDriver(const int EN1, const int IN1, const int IN2, const int IN3, const int IN4, const int EN2);
    void setup();
    void forward(int speed);
    void backward(int speed);
    void left(int speed);
    void right(int speed);
    void stop();
    void drive(char direction, int speed);

private:
    const int EN1;
    const int IN1;
    const int IN2;
    const int IN3;
    const int IN4;
    const int EN2;
};

#endif
