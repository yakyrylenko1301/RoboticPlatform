#include "speed_sensor.h"
#include <wiringPi.h>
#include <time.h>
#include <thread>
#include <bcm2835.h>
#include <sys/time.h>
#include <chrono>
#include <ctime>
#include <unistd.h>

namespace private_speed_sensor_cfg
{
    std::map<speed_sensor_cfg::posSpeedSensors, volatile int> holesCounter = 
    {
        {speed_sensor_cfg::FRONT_RIGHT, 0}, 
        {speed_sensor_cfg::FRONT_LEFT, 0}, 
        {speed_sensor_cfg::BACK_RIGHT, 0}, 
        {speed_sensor_cfg::BACK_LEFT, 0}
    };

    std::chrono::time_point<std::chrono::system_clock> prevTimeFrontRight;
    std::chrono::time_point<std::chrono::system_clock> prevTimeFrontLeft;
    std::chrono::time_point<std::chrono::system_clock> prevTimeBackRight;
    std::chrono::time_point<std::chrono::system_clock> prevTimeBackLeft;

    int filterTime = 700;
};

void speed_sensor_cfg::speedSensorFrontRightISR(void)
{
    auto now = std::chrono::system_clock::now();
    auto prev  = private_speed_sensor_cfg::prevTimeFrontRight;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - private_speed_sensor_cfg::prevTimeFrontRight);

    if(duration.count() >= private_speed_sensor_cfg::filterTime)
    {
        private_speed_sensor_cfg::holesCounter[speed_sensor_cfg::FRONT_RIGHT]++;
        private_speed_sensor_cfg::prevTimeFrontRight = std::chrono::system_clock::now();
    }
}

void speed_sensor_cfg::speedSensorFrontLeftISR(void)
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - private_speed_sensor_cfg::prevTimeFrontLeft);

    if(duration.count() >= private_speed_sensor_cfg::filterTime)
    {
        private_speed_sensor_cfg::holesCounter[speed_sensor_cfg::FRONT_LEFT]++;
        private_speed_sensor_cfg::prevTimeFrontLeft = std::chrono::system_clock::now();
    }
}

void speed_sensor_cfg::speedSensorBackRightISR(void)
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - private_speed_sensor_cfg::prevTimeBackRight);

    if(duration.count() >= private_speed_sensor_cfg::filterTime)
    {
        private_speed_sensor_cfg::holesCounter[speed_sensor_cfg::BACK_RIGHT]++;
        private_speed_sensor_cfg::prevTimeBackRight = std::chrono::system_clock::now();
    }
}

void speed_sensor_cfg::speedSensorBackLeftISR(void)
{
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - private_speed_sensor_cfg::prevTimeBackLeft);

    if(duration.count() >= private_speed_sensor_cfg::filterTime)
    {
        private_speed_sensor_cfg::holesCounter[speed_sensor_cfg::BACK_LEFT]++;
        private_speed_sensor_cfg::prevTimeBackLeft = std::chrono::system_clock::now();
    }
}

void* speed_sensor_cfg::threaHandlerFrontRight(void* arg)
{
    speedSensor* spd = static_cast<speedSensor*>(arg);
    const speed_sensor_cfg::posSpeedSensors pos = FRONT_RIGHT;

    int i = 0;
    for(;;)
    {
        if(i == 4)
        {
            i = 0;
            spd->calcRPM(private_speed_sensor_cfg::holesCounter[pos]);
            private_speed_sensor_cfg::holesCounter[pos] = 0; 
        }

        sleep(1);
        i++;
    } 
}
void* speed_sensor_cfg::threaHandlerFrontLeft(void* arg)
{
    speedSensor* spd = static_cast<speedSensor*>(arg);
    const speed_sensor_cfg::posSpeedSensors pos = FRONT_LEFT;

    int i = 0;
    for(;;)
    {
        if(i == 4)
        {
            i = 0;
            spd->calcRPM(private_speed_sensor_cfg::holesCounter[pos]);
            private_speed_sensor_cfg::holesCounter[pos] = 0; 
        }

        sleep(1);
        i++;
    } 
}
void* speed_sensor_cfg::threaHandlerBackRight(void* arg)
{
    speedSensor* spd = static_cast<speedSensor*>(arg);
    const speed_sensor_cfg::posSpeedSensors pos = BACK_RIGHT;

    int i = 0;
    for(;;)
    {
        if(i == 4)
        {
            i = 0;
            spd->calcRPM(private_speed_sensor_cfg::holesCounter[pos]);
            private_speed_sensor_cfg::holesCounter[pos] = 0; 
        }

        sleep(1);
        i++;
    } 
}
void* speed_sensor_cfg::threaHandlerBackLeft(void* arg)
{
    speedSensor* spd = static_cast<speedSensor*>(arg);
    const speed_sensor_cfg::posSpeedSensors pos = BACK_LEFT;

    int i = 0;
    for(;;)
    {
        if(i == 4)
        {
            i = 0;
            spd->calcRPM(private_speed_sensor_cfg::holesCounter[pos]);
            private_speed_sensor_cfg::holesCounter[pos] = 0; 
        }

        sleep(1);
        i++;
    }   
}

speedSensor::speedSensor(int pinSpeedSensor, void (*isr)(void), void *(*thread_func) (void *))
    : pinSpeedSensor(pinSpeedSensor)
{
    wiringPiSetup();
    pinMode(this->pinSpeedSensor, INPUT);
    wiringPiISR (this->pinSpeedSensor, INT_EDGE_BOTH, isr);
    this->opened = true;
    this->counerEncoderWheelHoles = 0;
    this->rpm = 0;
    this->calcInterval = 60 / 5;
    this->usInterval = 5 * 1000000;
    if (pthread_create(&this->thread, NULL, thread_func, this) != 0) 
    {
        this->opened = false;
    }  
}

void speedSensor::calcRPM(int count)
{
    this->rpm = (count * calcInterval) / speed_sensor_cfg::NbrEncoderWheelHoles;
}

int speedSensor::getRPM(void)
{
    return this->rpm;
}

bool speedSensor::isOpened(void)
{
    return this->opened;
}

speedSensor::~speedSensor(void)
{
    pthread_cancel(this->thread);    
}