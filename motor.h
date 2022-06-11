#ifndef MOTOR_H
#define MOTOR_H

// ######################################################################################
// # L298N Module 1
// # front motor right En1 = Gpio 22, In1 =  Gpio 27, In2 = Gpio 17 Speed sensor = 5 +
// # back  motor right En2 = Gpio 2,  In1 =  Gpio 3,  In2 = Gpio 4  Speed sensor = 10 +
// ######################################################################################

// ######################################################################################
// # L298N Module 2
// # front motor left En1 = Gpio 14, In1 =  Gpio 15, In2 = Gpio 18 Speed sensor = 11
// # back  motor left En2 = Gpio 25, In1 =  Gpio 24, In2 = Gpio 23 Speed sensor = 9
// ######################################################################################
namespace motorCtrl_cfg
{
    enum Diraction
    {
        FORWARF,
        STOP,
        BACK,
    };
};

class MotorCtrl
{
private:
    int pinCtrlIn1;
    int pinCtrlIn2;
    int pinPWM;
    int intensity;

public:
    MotorCtrl() = delete;
    MotorCtrl(const MotorCtrl&) = delete;
    MotorCtrl& operator=(const MotorCtrl&) = delete;
    MotorCtrl(int pinCtrlIn1, int pinCtrlIn2, int pinPWM);
    ~MotorCtrl(void);
    void speedUp(int intensity);
    void speedDown(int intensity);
    void forward(void);
    void stop(void);
    void back(void);
};

#endif