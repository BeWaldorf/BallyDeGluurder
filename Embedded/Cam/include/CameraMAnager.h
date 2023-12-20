#ifndef CameraManager_h
#define CameraManager_h

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#include "esp_camera.h"

class CameraManager
{
public:
    CameraManager(int FLASH_PIN, int PWM_FREQUENCY, int PWM_LED_CHANNEL, int PWM_RESOLUTION);
    bool initialize();
    void setFlash(int value);

private:
    int FLASH_PIN;
    int PWM_FREQUENCY;
    int PWM_LED_CHANNEL;
    int PWM_RESOLUTION;
};

#endif // CameraManager_h
