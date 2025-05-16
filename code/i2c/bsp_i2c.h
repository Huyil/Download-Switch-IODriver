#ifndef __bsp_i2c_H__
#define __bsp_i2c_H__

#include "cw32l011_i2c.h"

#define I2C_SLAVEADDRESS  0x54
#define I2C_SLAVEADDRESS1 0x56
#define I2C_SLAVEADDRESS2 0x58

void I2C_Configuration(void);

#endif //__bsp_i2c_H__


