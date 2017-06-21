#ifndef Includes_h
#define Includes_h

#include <Wire.h>
#include <EEPROM.h>
#include "../lib/I2Cdev/I2Cdev.h"
#include "../lib/MPU9150Lib/MPU9150Lib.h"
#include "../lib/CalLib/CalLib.h"
#include "../lib/MotionDriver/dmpKey.h"
#include "../lib/MotionDriver/dmpmap.h"
#include "../lib/MotionDriver/inv_mpu.h"
#include "../lib/MotionDriver/inv_mpu_dmp_motion_driver.h"

#include "../lib/Trekking/Trekking.hpp"
#include "../lib/Logger/PrintLogger.hpp"
#include "../lib/PIDController/PIDController.hpp"
#include "../lib/Filter/SimpleMovingAverageFilter.hpp"

#include "Pins.h"
#include "LonguinhoSensoring.hpp"
#include "LonguinhoMotorController.hpp"

#endif //Includes_h
