#include <QuaternionFilters.h>
#include <CmdMessenger.h>
#include <MPU9250.h>
#include <EEPROM.h>
#include "Messaging.h"
#include "Constants.h"
#include "Pins.h"
#include "Variables.h"

MPU9250 IMU(Wire, 0x68);
CmdMessenger messenger(Serial);

void setupIMU();
void retrieveReadings();
void filterReadings();
void processReadings();
void sendData();
void readMagCalibration();
void calibrateMagnetometer();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }

    pinMode(CALIBRATION_LED_PIN, OUTPUT);
    pinMode(CALIBRATION_BUTTON_PIN, INPUT);
    attachCallbacks(&messenger);
}

void setupIMU()
{
    int status = IMU.begin();
    if (status < 0)
    {
        while (1)
        {
            messenger.sendCmdStart(MessageCodes::kLog);
            messenger.sendCmdArg(LogMessageSubCodes::kError);
            messenger.sendCmdArg("Inicialização do MPU falhou");
            messenger.sendCmdArg(status);
            messenger.sendCmdEnd();
            delay(5000);
        }
    }

    // Configura o acelerômetro para acelerações de até +/-2G
    IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
    // Configura o giroscópio para velocidades de até +/-250 deg/s
    IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
    // Configura o Digital Low Pass Filter para filtrar frequências acima de 5Hz
    IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_5HZ);
    // Configura o SRD para 9 resultando numa taxa de atualiação de 50 Hz
    IMU.setSrd(9);

    IMU.calibrateAccel();
    IMU.calibrateGyro();
    readMagCalibration();
}

void loop()
{
    if (digitalRead(CALIBRATION_BUTTON_PIN) == HIGH)
    {
        calibrateMagnetometer();
        return;
    }

    if (IMU.readSensor())
    {
        retrieveReadings();
        filterReadings();
    }

    unsigned long now = micros();
    deltaTimeMicro = now - lastUpdate;
    deltaTimeSec = deltaTimeMicro / 1000000.0f; // set integration time by time elapsed since last filter update
    lastUpdate = now;

    processReadings();
    sendData();
    messenger.feedinSerialData();
}

void retrieveReadings()
{
    // Salva valores necessários para a integração
    lastAcceleration = acceleration;
    lastVelocity = velocity;

    acceleration.set(IMU.getAccelX_mss(), IMU.getAccelY_mss(), IMU.getAccelZ_mss());
    angularVelocity.set(IMU.getGyroX_rads(), IMU.getGyroY_rads(), IMU.getGyroZ_rads());
    magneticField.set(IMU.getMagX_uT(), IMU.getMagY_uT(), IMU.getMagZ_uT());
}

void filterReadings()
{
    // Simples filtro exponencial para tentar diminuir a oscilação do acelerômetro
    acceleration = Vector3::lerp(lastAcceleration, acceleration, 0.995);
}

void processReadings()
{
    MadgwickQuaternionUpdate(
        -acceleration.X, acceleration.Y, acceleration.Z,
        angularVelocity.X * PI_180, -angularVelocity.X * PI_180, -angularVelocity.Z * PI_180,
        magneticField.Y, -magneticField.X, magneticField.Z,
        deltaTimeSec);

    float yaw, pitch, roll;
    quaternion.toEulerAngles(&yaw, &pitch, &roll);
    transform.heading = yaw;

    // TODO
    // Precisamos remover a aceleração da gravidade nos outros eixos,
    // caso o MPU esteja inclinado a gravidade vai atuar nos eixos X e Y também.
    //
    // Podemos verificar as formulas de física 1 pra fazer isso.

    lastVelocity = velocity;
    velocity = ((acceleration + lastAcceleration) / 2.0f) * deltaTimeSec;

    transform.position = ((velocity + lastVelocity) / 2.0f) * deltaTimeSec;
}

void sendData()
{
    messenger.sendCmdStart(MessageCodes::kUpdateTransform);
    messenger.sendCmdArg(transform.position.X);
    messenger.sendCmdArg(transform.position.Y);
    messenger.sendCmdArg(transform.heading);
}

void readMagCalibration()
{
    Vector3 bias, magScale;
    EEPROM.get(0, bias);
    EEPROM.get(sizeof(Vector3), magScale);

    IMU.setMagCalX(bias.X, magScale.X);
    IMU.setMagCalY(bias.Y, magScale.Y);
    IMU.setMagCalZ(bias.Z, magScale.Z);
}

void calibrateMagnetometer()
{
    digitalWrite(CALIBRATION_LED_PIN, HIGH);
    IMU.calibrateMag();

    Vector3 bias;
    bias.X = IMU.getMagBiasX_uT();
    bias.X = IMU.getMagBiasY_uT();
    bias.X = IMU.getMagBiasZ_uT();

    Vector3 magScale;
    magScale.X = IMU.getMagScaleFactorX();
    magScale.X = IMU.getMagScaleFactorY();
    magScale.X = IMU.getMagScaleFactorZ();

    EEPROM.put(0, bias);
    EEPROM.put(sizeof(Vector3), magScale);
}
