#ifndef LonguinhoEncoder_hpp
#define LonguinhoEncoder_hpp

#include "../lib/Position/Position.hpp"
#include "../lib/WheelEncoder/WheelEncoder.hpp"
#include "LonguinhoMotorController.hpp"

class LonguinhoEncoder : public WheelEncoder
{
public:
  void initialize(LonguinhoMotorController *pMotorController)
  {
    m_pMotorController = pMotorController;
  }

  void update(Position *pPosition);


private:
  LonguinhoMotorController *m_pMotorController;
};

#endif //LonguinhoEncoder_hpp
