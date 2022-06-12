#ifndef SPEED_SENSOR_H
#define SPEED_SENSOR_H
#include <pthread.h>
#include <map>

namespace speed_sensor_cfg
{
    enum posSpeedSensors
    {
        FRONT_RIGHT,
        FRONT_LEFT,
        BACK_RIGHT,
        BACK_LEFT,
    };

    const int NbrEncoderWheelHoles = 40;

    void speedSensorFrontRightISR(void);
    void speedSensorFrontLeftISR(void);
    void speedSensorBackRightISR(void);
    void speedSensorBackLeftISR(void);


    void* threaHandlerFrontRight(void* arg);
    void* threaHandlerFrontLeft(void* arg);
    void* threaHandlerBackRight(void* arg);
    void* threaHandlerBackLeft(void* arg);


};

class speedSensor
{
private:
    int pinSpeedSensor; 
    unsigned int rpm;
    unsigned int counerEncoderWheelHoles;
    bool opened;
    pthread_t thread;
    unsigned long calcInterval;
    unsigned long usInterval; 
    
public:
    speedSensor() = delete;
    speedSensor(const speedSensor&) = delete;
    speedSensor& operator=(const speedSensor&) = delete;
    speedSensor(int pinSpeedSensor, void (*isr)(void), void *(*thread_func) (void *));
    ~speedSensor(void);
    int getRPM(void);
    bool isOpened(void);
    void calcRPM(int count);
};

#endif // SPEED_SENSOR_H