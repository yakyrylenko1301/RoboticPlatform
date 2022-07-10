#ifndef FDRM_STBC_AGM_01_H
#define FDRM_STBC_AGM_01_H
#include <stdint.h>
#include "FXAS21002CQ.h"
#include "FXOS8700CQ.h"

template <typename T>
struct data_coordinate
{
    T x;
    T y;
    T z;
};

/*
 * 6-Axis Sensor with Integrated Linear Accelerometer and Magnetometer
 *
 */
class FXOS8700CQ
{
private:
    uint8_t i2c_addr;
    int fd;
    // Sensor configuration
	uint8_t accelFSR;
	uint8_t accelODR;
	uint8_t magOSR;
    // Resolution
	float getAres(void);
	float getMres(void);

    // FXOS8700CQ functions
	// Initialization & Termination

	void standby(void);
	void active(void);

    // Sensor data

	int8_t tempData; // RAW temperature data
public:
    FXOS8700CQ(uint8_t i2c_addr);
    void writeReg(uint8_t reg, uint8_t value);
	uint8_t readReg(uint8_t reg);
    void readAccelData(void);
	void readMagData(void);
	void readTempData(void);

    bool isOpen(void);
    void init(void);
    data_coordinate<int> accelData; // RAW acceleration sensor data
	data_coordinate<int> magData; // RAW magnometer sensor data

};

#endif // !FDRM_STBC_AGM_01_H
