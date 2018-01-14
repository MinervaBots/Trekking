#include "CommandHandlers.h"
#include "Variables.h"
#include "Constants.h"

unsigned long prevTime = millis(); // Tempo da última leitura
Vector2 prevAcceleration;
Vector2 prevVelocity;

void onRecvMpuData(CmdMessenger *cmdMesseger)
{
  // Adquire os dados enviados pelo MPU
  float accelerationX = cmdMesseger->readBinArg<float>();
  float accelerationY = cmdMesseger->readBinArg<float>();
  float heading = cmdMesseger->readBinArg<float>();
  
  unsigned long deltaTimeInSeconds = (millis() - prevTime) / 1000; // 1000 adicionado para obter as mesmas unidades
  
  // Integra por trapézios a aceleração para ober a velocidade em cada eixo
  currentVelocity.x += INTEGRATION_TERM(prevAcceleration.x, accelerationX, deltaTimeInSeconds);
  currentVelocity.y += INTEGRATION_TERM(prevAcceleration.y, accelerationY, deltaTimeInSeconds);

  float xVar, yVar, newHeading = 0;

#ifdef USE_KINAMTICS_MODEL
  // Calcula a variação de posição usando um modelo de triciclo
  float servoHeading = radians(steeringServo.read())
  xVar = currentVelocity.x * cos(servoHeading)
  yVar = currentVelocity.y * cos(servoHeading)
  newHeading = currentTransform.heading + (currentVelocity.module() / DISTANCE_BETWEEN_AXES) * servoHeading;
#else
  // Calcula a variação de posição usando a integração da velocidade
  // Futuramente todos esses calculos devem ser movidos para o MPU
  xVar = INTEGRATION_TERM(currentVelocity.x, prevVelocity.x, deltaTimeInSeconds) * cos(heading);
  yVar = INTEGRATION_TERM(currentVelocity.y, prevVelocity.y, deltaTimeInSeconds) * cos(heading);
  newHeading = heading;
#endif

  // Aplica as variações calculadas
  currentTransform.position.x += xVar;
  currentTransform.position.y += yVar;
  currentTransform.heading = newHeading;

  // Salva os ultimos valores de aceleração
  prevAcceleration.x = accelerationX;
  prevAcceleration.y = accelerationY;

  // Salva os ultimos valores de velocidade
  prevVelocity.x = currentVelocity.x;
  prevVelocity.y = currentVelocity.y;

  // Salva o tempo da ultima execução para usar na integração
  prevTime = millis();
}

void onRecvTargetData(CmdMessenger *cmdMesseger)
{
  targetDirection = cmdMesseger->readBinArg<double>();
  targetDirectionFiltered.add(targetDirection);
  
  //int x = cmdMesseger->readBinArg<int>();
  //int y = cmdMesseger->readBinArg<int>();
  //int w = cmdMesseger->readBinArg<int>();
  int h = cmdMesseger->readBinArg<int>();

  targetDirectionFiltered.add((FOCAL_LENGHT * CONE_REAL_HEIGHT * IMAGE_PIXEL_HEIGHT) / (h * SENSOR_HEIGHT));
  cmdMesseger->sendCmd(info, "Dados recebidos");
}

void onRecvUnknownCommand(CmdMessenger *cmdMesseger)
{
  // Algum buzzer futuramente
}

