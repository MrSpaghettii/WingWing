#include <Arduino.h>
#include "IMU.h"
// #include "PID.h"

// Instantiate our sensor object
IMU myIMU;

void setup() {
    Serial.begin(115200);
    // Wait 2 seconds to ensure the serial monitor is open before printing
    delay(2000); 
    
    Serial.println("\n--- Flight Controller Booting ---");

    // Initialize the IMU
    myIMU.init();
}

void loop() {
    // 1. Read sensors
    myIMU.update();
    
    // Print the values that the class is holding
    Serial.printf("Main Loop - Roll: %.2f | Pitch: %.2f\n", myIMU.getRoll(), myIMU.getPitch());

    // 2. Compute PID (Future step)
    // flightPID.compute(myIMU.getRoll(), myIMU.getPitch());
    
    // 3. Command Motors (Future step)
    // motors.apply(flightPID.getOutput());

    // Delay for readability in serial monitor. 
    // In a real flight controller, this will be replaced by a precise microsecond timer loop.
    delay(50); 
}