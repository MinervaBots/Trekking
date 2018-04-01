#include <SparkFunMPU9250-DMP.h>
#include "Vector.h"

#define G 9.8
#define BAUD_RATE 9600
#define CALIBRATION_BUTTON 10

MPU9250_DMP imu;
Vector3 acceleration, inertialAcceleration;
Vector3 gyroscope, gyroBias;
Vector3 compass, compassBias;

void setupMPU();

void setup()
{
  Serial.begin(BAUD_RATE);
  imu.begin();
  setupMPU();
}

void loop()
{
  if (imu.dataReady())
  {
    readMPU();
  }
}

void setupMPU()
{
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  imu.setAccelFSR(4); // Set accel to +/-4g
  imu.setLPF(5); // Set LPF corner frequency to 5Hz
  imu.setCompassSampleRate(100); // Set mag rate to 100Hz

  // Calcula valores de referência com o robô parado
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
  acceleration.X = imu.calcAccel(imu.ax);
  acceleration.Y = imu.calcAccel(imu.ay);
  acceleration.Z = imu.calcAccel(imu.az);
  
  acceleration.X = imu.calcGyro(imu.gx);
  acceleration.Y = imu.calcGyro(imu.gy);
  acceleration.Z = imu.calcGyro(imu.gz);
  
  acceleration.X = imu.calcMag(imu.mx);
  acceleration.Y = imu.calcMag(imu.my);
  acceleration.Z = imu.calcMag(imu.mz);
}

void readMPU()
{
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
  
  acceleration.X = imu.calcAccel(imu.ax);
  acceleration.Y = imu.calcAccel(imu.ay);
  acceleration.Z = imu.calcAccel(imu.az);
  
  acceleration.X = imu.calcGyro(imu.gx);
  acceleration.Y = imu.calcGyro(imu.gy);
  acceleration.Z = imu.calcGyro(imu.gz);
  
  acceleration.X = imu.calcMag(imu.mx);
  acceleration.Y = imu.calcMag(imu.my);
  acceleration.Z = imu.calcMag(imu.mz);
}