#include "RPiMessageHandlers.h"

using namespace raspberryPiMessages;

FirmataClass raspberryPiFirmata;

void raspberryPiIntMessage(byte messageCode, int value)
{
  switch(messageCode)
  {
    case MessageCodes::SetTrakkingDirection:
      setTrakkingDirection(value);
      break;
      
    case MessageCodes::SetMaxVelocity:
      setMaxVelocity(value);
      break;
      
    default:
      // Responde ao RPi que não temos nenhum handler pra esse código
      raspberryPiFirmata.sendString("Nenhum handler declarado para o código dessa mensagem: "); // TODO - Inserir o código da mensagem
      break;
  }
}

namespace raspberryPiMessages
{
  void setTrakkingDirection(int value)
  {
    float direction = map(value, 0, 16383, -1.0, 1.0);
  }

  void setMaxVelocity(int value)
  {
     
  }
};

