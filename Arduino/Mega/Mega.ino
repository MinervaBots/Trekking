

#include "RPiMessageHandlers.h"

void setup()
{
  Serial.begin(57600);
  raspberryPiFirmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
  
  // Usando o handler para ANALOG_MESSAGE, podemos ter até 15 handlers para inteiros, e uma resolução de 16383 para ao valor.
  // Convertando essa resolução pra float (variando entre -1 e 1), temos 0.00012207776 de resolução que é muito mais do que precisamos
  raspberryPiFirmata.attach(ANALOG_MESSAGE, raspberryPiIntMessage);
  
  raspberryPiFirmata.begin(Serial);
}

void loop()
{
  while (raspberryPiFirmata.available())
  {
    raspberryPiFirmata.processInput();
  }
  // processSensors();
}
