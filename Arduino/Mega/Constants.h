#ifndef CONSTANTS_H
#define CONSTANTS_H


#define RPI_BAUD_RATE         9600
#define MPU_BAUD_RATE         9600

#define CONE_REAL_HEIGHT      0.5  // Cone height: 50 cm = 0.5 m
#define FOCAL_LENGHT          3.04 // Focal lenght: 3.04 mm
#define SENSOR_HEIGHT         3.68 // Sensor image area: 3.68 x 2.76 mm (4.6 mm diagonal)
#define IMAGE_PIXEL_HEIGHT    3280 // Height size of the image in sensor: 3280 pixels

#define CAMERA_SERVO_LIMIT    180
#define STEERING_SERVO_LIMIT  90
#define ESC_PID_LIMIT         90 // O ESC é usado como um servo, logo 180 representa velocidade máxima


#endif
