#include <CmdMessenger.h>
#include <SparkFunMPU9250-DMP.h>
//#include <EEPROM.h>

//#define E 2.7
#define G 9.8

//#define DECAY 10000 // constante de decaimento para função exponecial usada no cálculo de Km

#define CALIBRATION_BUTTON 10

enum
{
    mpuData,
    error,
};

const int BAUD_RATE = 9600;
CmdMessenger c = CmdMessenger(Serial,',',';','/');
MPU9250_DMP imu;

float ax, ay, gyro;// mx, my;
float Km; // quanto mais próximo a 1, maior o peso do magnetômetro, quanto mais próximo a 0, maior o peso do giroscópio
float offsetGyro, offsetAx, offsetAy, lastGyro;// offsetMag;
float gyroHeading; // ângulo calculado pelo giroscópio
//float magHeading; // ângulo calculado pelo magnetômetro
long lastRun; // guarda a última vez que o giroscópio foi calculado (para usar o dt na integral)
//float minMxReal, maxMxReal, minMyReal, maxMyReal;
/*
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/

float accelerationX, accelerationY, heading;

void readMPU();
void calculateData();
void calibrateGyroAccel();
void calibrateGyroAccel();
void setMPU();
//void magnetometerCalibration();

void attachHandlers();
void onUnknownCommand();
void sendData();

void setup()
{
  Serial.begin(BAUD_RATE);
  attachHandlers();
  imu.begin();
  //magnetometerCalibration();
  setMPU();
  calibrateGyroAccel();
}

void loop()
{
  readMPU();
  calculateData();
}

void onUnknownCommand()
{
  c.sendCmd(error, "Comando sem handler definido");
}

void attachHandlers()
{
  c.attach(onUnknownCommand);
}

void sendData()
{
  c.sendCmdStart(mpuData);
  c.sendCmdBinArg(accelerationX);
  c.sendCmdBinArg(accelerationY);
  c.sendCmdBinArg(heading);
  c.sendCmdEnd();  
}

void readMPU()
{
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
  ax = imu.calcAccel(imu.ax);
  ay = imu.calcAccel(imu.ay);
  gyro = imu.calcAccel(imu.gz);
  //mx = imu.calcAccel(imu.mx);
  //my = imu.calcAccel(imu.my);
}

void calculateData()
{
    /*
  float mxReal = mapFloat(mx, minMxReal, maxMxReal, -1000, 1000);
  float myReal = mapFloat(my, minMyReal, maxMyReal, -1000, 1000);
  magHeading = atan2(mxReal,myReal) - offsetMag;
  magHeading *= (180 / PI);
  */
  gyroHeading -= (lastGyro + (gyro - offsetGyro)) * (micros() - lastRun) / 4000; // integrando a velocidade angular -> ângulo
  if (gyroHeading > 180.0) {
    gyroHeading -= 360.0;
  }
  if (gyroHeading < -180.0){
    gyroHeading += 360.0;
  }
  /*
  if (magHeading > 180.0) {
    magHeading -= 360.0;
  }
  if (magHeading < -180.0){
    magHeading += 360.0;
  }
  Km = pow(E, -DECAY * abs((gyro - offsetGyro))); // função que encontra Km. Quanto maior a velocidade ângular, maior o peso do gyroscópio
  //gyroHeading = Km * magHeading + (1 - Km) * gyroHeading; // filtro sendo feito e já atualizando o valor de gyroHeading
  */
  accelerationX = (ax - offsetAx) * G; //sqrt(pow((ax - offsetAx),2) + pow((ay - offsetAy),2))*G;
  accelerationY = (ay - offsetAy) * G; //sqrt(pow((ax - offsetAx),2) + pow((ay - offsetAy),2))*G;
  heading = gyroHeading;
  
  lastRun = micros();
  lastGyro = gyro - offsetGyro;
  //Serial.println(int(gyroHeading));
}

// encontrar offsets
void calibrateGyroAccel()
{
  //offsetMag = 0;
  gyroHeading = 0;
  offsetGyro = 0;
  lastGyro = 0;
  for (int i = 0; i < 1000; i++) {
    readMPU();
    offsetGyro += gyro;
    offsetAx += ax;
    offsetAy += ay;
    //float mxReal = mapFloat(mx, minMxReal, maxMxReal, -1000, 1000);
    //float myReal = mapFloat(my, minMyReal, maxMyReal, -1000, 1000);
    //offsetMag += atan2(mxReal,myReal);
  }
  offsetGyro /= 1000.0;
  offsetAx /= 1000.0;
  offsetAy /= 1000.0;
  //offsetMag /= 1000.0;
  lastRun = micros();
}

void reset()
{
  gyroHeading = 0;
  lastRun = micros();
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
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
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
  imu.setSampleRate(100); // Set sample rate to 100Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(100); // Set mag rate to 100Hz
  //EEPROM.get(0, minMxReal);
  //EEPROM.get(sizeof(float), maxMxReal);
  //EEPROM.get(2*sizeof(float), minMyReal);
  //EEPROM.get(3*sizeof(float), maxMyReal);
}

/*
void magnetometerCalibration() {
  float minMx, maxMx, minMy, maxMy;
  delay(5000);
  readMPU();
  minMx = imu.calcAccel(imu.mx);
  maxMx = imu.calcAccel(imu.mx);
  minMy = imu.calcAccel(imu.my);
  maxMy = imu.calcAccel(imu.my);
  long start = millis();
  while(millis() - start < 50000) {
    readMPU();
    minMx = min(minMx, imu.calcAccel(imu.mx));
    maxMx = max(maxMx, imu.calcAccel(imu.mx));
    minMy = min(minMy, imu.calcAccel(imu.my));
    maxMy = max(maxMy, imu.calcAccel(imu.my));
    Serial.print("minMx = ");
    Serial.print(minMx*10000000000);
    Serial.print("     maxMx = ");
    Serial.print(maxMx*10000000000);
    Serial.print("     minMy = ");
    Serial.print(minMy*10000000000);
    Serial.print("     maxMy = ");
    Serial.print(maxMy*10000000000);
    Serial.print("     ");
    Serial.println(50000 - (millis() - start));
  }
  EEPROM.put(0, minMx);
  EEPROM.put(sizeof(float), maxMx);
  EEPROM.put(2*sizeof(float), minMy);
  EEPROM.put(3*sizeof(float), maxMy);
}
*/
