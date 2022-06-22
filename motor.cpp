#include "motor.h"
#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

MotorCtrl::MotorCtrl(int pinCtrlIn1, int pinCtrlIn2, int pinPWM):
    pinCtrlIn1(pinCtrlIn1), pinCtrlIn2(pinCtrlIn2), pinPWM(pinPWM)
{
    wiringPiSetup();
    pinMode(pinPWM, OUTPUT);
    pinMode(pinCtrlIn1, OUTPUT);
    pinMode(pinCtrlIn2, OUTPUT);
    softPwmCreate(pinPWM, 1, 100);
    softPwmWrite(pinPWM, 0);
    digitalWrite(pinCtrlIn1, LOW);
    digitalWrite(pinCtrlIn2, LOW); 
    this->intensity = 0;
}

void MotorCtrl::speedUp(int pwm_intensity)
{
    this->intensity = pwm_intensity;
    softPwmWrite(pinPWM, this->intensity);
}

void MotorCtrl::speedDown(int pwm_intensity)
{
    this->intensity = pwm_intensity;
    softPwmWrite(pinPWM, this->intensity);
}

void MotorCtrl::stop(void)
{ 
    this->intensity = 0;
    softPwmWrite(pinPWM, this->intensity);   
    digitalWrite(pinCtrlIn1, LOW);
    digitalWrite(pinCtrlIn2, LOW); 
}

void MotorCtrl::forward(void)
{
    digitalWrite(pinCtrlIn1, LOW);
    digitalWrite(pinCtrlIn2, HIGH);
    softPwmWrite(pinPWM, this->intensity);
}

void MotorCtrl::back(void)
{
    digitalWrite(pinCtrlIn1, HIGH);
    digitalWrite(pinCtrlIn2, LOW);
    softPwmWrite(pinPWM, this->intensity);
}


MotorCtrl::~MotorCtrl()
{

}
