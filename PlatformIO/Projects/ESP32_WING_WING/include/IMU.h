//init for all mpu6050
#define IMU_H

#include <Arduino.h>

class IMU {
private:
    float roll;
    float pitch;
    bool initialized;

public:
    IMU();
    
    // Call this in setup() to configure the MPU-6050
    bool init();
    
    void update();

    // for the PID controller to access
    float getRoll() const;
    float getPitch() const;
};