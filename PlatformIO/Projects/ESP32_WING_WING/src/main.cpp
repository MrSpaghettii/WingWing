#include <Arduino.h>

void setup() {
    // Initialize the USB Serial port
    Serial.begin(115200);
    // Wait up to 2 seconds for the serial monitor to open
    unsigned long start = millis();
    while (!Serial && (millis() - start < 2000));
    
    Serial.println("--- Code-OSS Environment Check ---");
    Serial.println("ESP32-S3 Core: Initialized successfully.");
}

void loop() {
    Serial.println("Flight controller loop running...");
    delay(1000);
}