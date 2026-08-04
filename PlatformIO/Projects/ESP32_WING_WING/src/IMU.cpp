#include "IMU.h"
#include <Wire.h>

IMU::IMU() : roll(0.0f), pitch(0.0f), initialized(false) {}

bool IMU::init() {
    // Initialize I2C on SDA=8, SCL=9
    Wire.begin(8, 9);
    
    // Set a timeout so the ESP32 doesn't freeze if a wire is loose
    Wire.setTimeOut(100); 
    
    Serial.println("Scanning for MPU-6050 Gyro...");
    
    // Check if the sensor exists at the standard 0x68 address
    Wire.beginTransmission(0x68);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
        Serial.println("SUCCESS: MPU-6050 Found!");
        // open i2c communication
        Wire.beginTransmission(0x68);
        // I want to change thispower management register to 0
        Wire.write(0x6B);
        //sleep no more
        Wire.write(0);
        //to actually receive the data EndTransmission must be called
        Wire.endTransmission();
        initialized = true;
        return true;
    } else {
        Serial.printf("ERROR: MPU-6050 missing. I2C Error Code: %d\n", error);
        Serial.println("Check wire configuration: (3v3 - VCC)  (port #8 - SDA) (port #9 - SCL) (GND - GND)");
        initialized = false;
        return false;
    }
}

void IMU::update() {
    if (!initialized) {
        Serial.println("Waiting for Gyro connection...");
        return;
    }

    // Only try to read data if the sensor is actually connected
    Wire.beginTransmission(0x68);
    if (Wire.endTransmission() == 0) {
        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);

        //fetching axis; they are stored in 2 bytes each, so we need to read 6 bytes total for X, Y, Z
        Wire.requestFrom((uint16_t)0x68, (uint8_t)6, true);
        
        //  we shift the first byte to the left
        int16_t AcX = Wire.read() << 8 | Wire.read();
        int16_t AcY = Wire.read() << 8 | Wire.read();
        int16_t AcZ = Wire.read() << 8 | Wire.read();
    
        // calcul du roulement et du pitch en degrés
        roll = atan2(AcY, AcZ) * 180 / PI;
        pitch = atan2(-AcX, sqrt(AcY * AcY + AcZ * AcZ)) * 180 / PI;

        // Optionally print for debugging, but usually you want to remove this in production flight code
        // Serial.printf("Roll: %.2f | Pitch: %.2f\n", roll, pitch);
    } else {
        Serial.println("I2C Read Error during update");
    }
}

float IMU::getRoll() const {
    return roll;
}

float IMU::getPitch() const {
    return pitch;
}