#include <Arduino_LSM9DS1.h>

float accelMagnitudePrev = 0;
float movementThreshold = 0.2; // Define a threshold for movement detection
float alpha = 0.5; // Complementary filter gain. Adjust between 0 and 1 as needed.
float pitch, roll, yaw;

void setup() {
  Serial.begin(9600);
  while (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    delay(1000);
  }
}

void loop() {
  float accX, accY, accZ, gyroX, gyroY, gyroZ;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);
    IMU.readGyroscope(gyroX, gyroY, gyroZ);

    // Calculate pitch, roll and yaw
    float accPitch = atan2(accY, sqrt(accZ*accZ + accX*accX)) * 180.0 / PI;
    float accRoll = atan2(accX, sqrt(accY*accY + accZ*accZ)) * 180.0 / PI;
    float accYaw = atan2(sqrt(accX*accX + accY*accY), accZ) * 180.0 / PI;

    if(isnan(pitch)){
      pitch = accPitch;
      roll = accRoll;
      yaw = accYaw;
    }else{
      pitch = pitch + gyroX * 0.001; // Assuming loop time is 1 ms
      roll = roll - gyroY * 0.001; // Assuming loop time is 1 ms
      yaw = yaw + gyroZ * 0.001; // Assuming loop time is 1 ms

      // Apply complementary filter
      pitch = pitch * alpha + accPitch * (1.0 - alpha);
      roll = roll * alpha + accRoll * (1.0 - alpha);
      yaw = yaw * alpha + accYaw * (1.0 - alpha);
    }

    Serial.print("Pitch: "); Serial.print(pitch);
    Serial.print("\tRoll: "); Serial.print(roll);
    Serial.print("\tYaw: "); Serial.println(yaw);

    // Calculate the magnitude of acceleration
    float accelMagnitude = sqrt(accX*accX + accY*accY + accZ*accZ) - 1.0; // Subtract 1g (gravity)
    accelMagnitude = abs(accelMagnitude - accelMagnitudePrev);
    accelMagnitudePrev = accelMagnitude;

    if (accelMagnitude > movementThreshold) {
      Serial.println("Movement detected!");
    }
  }

  delay(1); // Small delay to simulate loop time for gyro integration
}
