#include <SparkFunMPU9250-DMP.h>
#include <PacketSerial.h>
#include <EEPROM.h>

#define E 2.7
#define G 9.8

#define DECAY 1 // Constante de decaimento para função exponecial usada no cálculo de Km.

#define CALIBRATION_BUTTON 10

MPU9250_DMP imu;

float ax, ay, gyro, mx, my;
float Km; // Quanto mais próximo a 1, maior o peso do magnetômetro. Quanto mais próximo a 0, maior o peso do giroscópio.
float offsetMag, offsetGyro, offsetAx, offsetAy;
float gyroHeading; // Ângulo calculado pelo giroscópio.
float magHeading; // Ângulo calculado pelo magnetômetro.
int lastRun; // Guarda a última vez que o giroscópio foi calculado para usar o dt na integral.

union
{
  float value;
  byte buffer[4];
} acceleration;

union
{
  float value;
  byte buffer[4];
} heading;


PacketSerial serial;

void readMPU() 
{
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
  ax = imu.calcAccel(imu.ax);
  ay = imu.calcAccel(imu.ay);
  gyro = imu.calcAccel(imu.gz) * PI / 180.0;
  mx = imu.calcAccel(imu.mx);
  my = imu.calcAccel(imu.my);
}

void reset() // Encontrar offsets.
{
  offsetMag = 0;
  gyroHeading = 0;
  offsetGyro = 0;
  
  for (int i = 0; i < 1000; i++) 
  {
    readMPU();
    offsetGyro += gyro;
    offsetAx += ax;
    offsetAy += ay;
    offsetMag += atan2(mx,my);
  }
  
  offsetGyro /= 1000.0;
  offsetAx /= 1000.0;
  offsetAy /= 1000.0;
  offsetMag /= 1000.0;
  lastRun = millis();
}

void receivedPacket(const uint8_t* buffer, size_t size)
{
  byte operationCode = (byte)buffer[0];
  switch (operationCode)
  {
    case 1:
      reset();
      break;
      
    case 2:
      byte response[10];
      response[0] = operationCode;
      memcpy(response[1], acceleration.buffer, sizeof(float));
      memcpy(response[1 + sizeof(float)], heading.buffer, sizeof(float));
      serial.send(response, 10);
      break;
  }
}

void setMPU() 
{
  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  
  imu.setGyroFSR(500); // Set gyro to 500 dps
  
  // Accel options are +/- 2, 4, 8, or 16 g
  
  imu.setAccelFSR(4); // Set accel to +/-4g
  
  // Note: the MPU-9250's magnetometer FSR is set at 
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  
  imu.setLPF(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  
  imu.setSampleRate(10); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  
  imu.setCompassSampleRate(10); // Set mag rate to 10Hz
}

void magnetometerCalibration() 
{
  float minMx, maxMx, minMy, maxMy;
  readMPU();
  minMx = imu.calcAccel(imu.mx);
  maxMx = imu.calcAccel(imu.mx);
  minMy = imu.calcAccel(imu.my);
  maxMy = imu.calcAccel(imu.my);
  
  while(digitalRead(CALIBRATION_BUTTON))
  {
    readMPU();
    minMx = min(minMx, imu.calcAccel(imu.mx));
    maxMx = max(maxMx, imu.calcAccel(imu.mx));
    minMy = min(minMy, imu.calcAccel(imu.my));
    maxMy = max(maxMy, imu.calcAccel(imu.my));
  }
  
  EEPROM.put(0, minMx);
  EEPROM.put(sizeof(float), maxMx);
  EEPROM.put(2*sizeof(float), minMy);
  EEPROM.put(3*sizeof(float), maxMy);
}

void setup() 
{
  imu.begin();
  setMPU();
  serial.begin(9600);
  serial.setPacketHandler(&receivedPacket);
  reset();
}

void loop()
{
  serial.update();
  readMPU();
  magHeading = atan2(mx, my) - offsetMag;
  gyroHeading += (gyro - offsetGyro) * (millis() - lastRun); // Integrando a velocidade angular para obter ângulo.
  Km = pow(E, -DECAY * abs((gyro - offsetGyro))); // Função que encontra Km. Quanto maior a velocidade ângular, maior o peso do giroscópio.
  gyroHeading = Km * magHeading + (1 - Km) * gyroHeading; // Filtro sendo feito e já atualizando o valor de gyroHeading.
  acceleration.value = sqrt(pow((ax - offsetAx), 2) + pow((ay - offsetAy), 2))  *G;
  heading.value = gyroHeading;
}
