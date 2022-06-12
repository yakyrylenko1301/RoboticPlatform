#include "electricDrive.h"


ElectricDrive::ElectricDrive(int x)
{
    this->motor_back_left = new MotorCtrl(ed_pin_wPi_cfg::BACK_LEFT::PIN_IN_1,
        ed_pin_wPi_cfg::BACK_LEFT::PIN_IN_2, ed_pin_wPi_cfg::BACK_LEFT::PIN_PWM);
    
    this->motor_back_right = new MotorCtrl(ed_pin_wPi_cfg::BACK_RIGHT::PIN_IN_1,
        ed_pin_wPi_cfg::BACK_RIGHT::PIN_IN_2, ed_pin_wPi_cfg::BACK_RIGHT::PIN_PWM);

    this->motor_front_left = new MotorCtrl(ed_pin_wPi_cfg::FRONT_LEFT::PIN_IN_1,
        ed_pin_wPi_cfg::FRONT_LEFT::PIN_IN_2, ed_pin_wPi_cfg::FRONT_LEFT::PIN_PWM);

    this->motor_front_right = new MotorCtrl(ed_pin_wPi_cfg::FRONT_RIGHT::PIN_IN_1,
        ed_pin_wPi_cfg::FRONT_RIGHT::PIN_IN_2, ed_pin_wPi_cfg::FRONT_RIGHT::PIN_PWM);

    this->ss_back_left = new speedSensor(ed_pin_wPi_cfg::BACK_LEFT::PIN_SPEED_SENSOR,
        speed_sensor_cfg::speedSensorBackLeftISR, speed_sensor_cfg::threaHandlerBackLeft);

    this->ss_back_right = new speedSensor(ed_pin_wPi_cfg::BACK_RIGHT::PIN_SPEED_SENSOR,
        speed_sensor_cfg::speedSensorBackRightISR, speed_sensor_cfg::threaHandlerBackRight);

    this->ss_front_left = new speedSensor(ed_pin_wPi_cfg::FRONT_LEFT::PIN_SPEED_SENSOR,
        speed_sensor_cfg::speedSensorFrontLeftISR, speed_sensor_cfg::threaHandlerFrontLeft);

    this->ss_front_right = new speedSensor(ed_pin_wPi_cfg::FRONT_RIGHT::PIN_SPEED_SENSOR,
        speed_sensor_cfg::speedSensorFrontRightISR, speed_sensor_cfg::threaHandlerFrontRight);

    
 
}

ElectricDrive::~ElectricDrive(void)
{
    delete this->motor_back_left;
    delete this->motor_back_right;
    delete this->motor_front_left;
    delete this->motor_front_right;
}