#ifndef CameraManager_h
#define CameraManager_h

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#include "esp_camera.h"

class CameraManager
{
public:
    CameraManager(int FLASH_PIN);
    bool initialize();
    void setFlash(int value);

private:
    int FLASH_PIN;
};

#endif // CameraManager_h
