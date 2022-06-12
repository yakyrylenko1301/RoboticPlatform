#ifndef ELECTRIC_DRIVE_H
#define ELECTRIC_DRIVE_H
#include "motor.h"
#include "speed_sensor.h"

namespace ed_pin_bcm_cfg
{
    namespace FRONT_RIGHT
    {
        const int PIN_PWM          = 22;
        const int PIN_IN_1         = 27;
        const int PIN_IN_2         = 17;
        const int PIN_SPEED_SENSOR = 5;
    };

    namespace BACK_RIGHT
    {
        const int PIN_PWM          = 2;
        const int PIN_IN_1         = 3;
        const int PIN_IN_2         = 4;
        const int PIN_SPEED_SENSOR = 10;
    };

    namespace FRONT_LEFT
    {
        const int PIN_PWM          = 14;
        const int PIN_IN_1         = 15;
        const int PIN_IN_2         = 18;
        const int PIN_SPEED_SENSOR = 11;
    };

    namespace BACK_LEFT
    {
        const int PIN_PWM          = 25;
        const int PIN_IN_1         = 24;
        const int PIN_IN_2         = 23;
        const int PIN_SPEED_SENSOR = 9;
    };
};

namespace ed_pin_wPi_cfg
{
    namespace FRONT_RIGHT
    {
        const int PIN_PWM          = 3;
        const int PIN_IN_1         = 2;
        const int PIN_IN_2         = 0;
        const int PIN_SPEED_SENSOR = 21;
    };

    namespace BACK_RIGHT
    {
        const int PIN_PWM          = 8;
        const int PIN_IN_1         = 9;
        const int PIN_IN_2         = 7;
        const int PIN_SPEED_SENSOR = 12;
    };

    namespace FRONT_LEFT
    {
        const int PIN_PWM          = 15;
        const int PIN_IN_1         = 16;
        const int PIN_IN_2         = 1;
        const int PIN_SPEED_SENSOR = 14;
    };

    namespace BACK_LEFT
    {
        const int PIN_PWM          = 6;
        const int PIN_IN_1         = 5;
        const int PIN_IN_2         = 4;
        const int PIN_SPEED_SENSOR = 13;
    };
};

class ElectricDrive
{
private:
    MotorCtrl* motor_front_right;
    MotorCtrl* motor_front_left;
    MotorCtrl* motor_back_right;
    MotorCtrl* motor_back_left;
    speedSensor* ss_front_right;
    speedSensor* ss_front_left;
    speedSensor* ss_back_right;
    speedSensor* ss_back_left;

    void speedSensorFrontRightISR(void);
    void speedSensorFrontLeftISR(void);
    void speedSensorBackRightISR(void);
    void speedSensorBackLeftISR(void);
public:
    ElectricDrive() = delete;
    ElectricDrive(const ElectricDrive&) = delete;
    ElectricDrive& operator=(const ElectricDrive&) = delete;
    ElectricDrive(int x);
    ~ElectricDrive(void);
};

#endif
