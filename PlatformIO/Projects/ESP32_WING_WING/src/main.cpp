#include <Arduino.h>
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    // Wait 2 seconds to ensure the serial monitor is open before printing
    delay(2000); 
    
    Serial.println("\n--- Flight Controller Booting ---");

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

        Wire.endTransmission();
    } else {
        Serial.printf("ERROR: MPU-6050 missing. I2C Error Code: %d\n", error);
        Serial.println("Check wire configuration: (3v3 - VCC)  (port #8 - SDA) (port #9 - SCL) (GND - GND)");
    }
}

void loop() {
    // Only try to read data if the sensor is actually connected
    Wire.beginTransmission(0x68);
    if (Wire.endTransmission() == 0) {
        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        
        Wire.requestFrom((uint16_t)0x68, (uint8_t)6, true);
        
        int16_t AcX = Wire.read() << 8 | Wire.read();
        int16_t AcY = Wire.read() << 8 | Wire.read();
        int16_t AcZ = Wire.read() << 8 | Wire.read();
        
        Serial.printf("Accel X: %d | Y: %d | Z: %d\n", AcX, AcY, AcZ);
    } else {
        Serial.println("Waiting for Gyro connection...");
    }
    
    delay(500);
}