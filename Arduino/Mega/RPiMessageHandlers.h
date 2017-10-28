#ifndef RPI_MESSAGE_HANDLERS_H
#define RPI_MESSAGE_HANDLERS_H

#include <Firmata.h>


using namespace firmata;
extern FirmataClass raspberryPiFirmata;


void raspberryPiIntMessage(byte messageCode, int value);

namespace raspberryPiMessages
{
  // Podemos ter até 15 mensagens
  enum MessageCodes
  {
    SetTrakkingDirection,
    SetMaxVelocity, 
  };
  // Declarar as mensagens aqui dentro pra manter o escopo bem definido

  void setTrakkingDirection(int value);
  void setMaxVelocity(int value);
};


#endif //RPI_MESSAGE_HANDLERS_H
