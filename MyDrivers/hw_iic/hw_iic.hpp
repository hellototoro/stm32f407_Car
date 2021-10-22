/*
 * hw_iic.hpp
 *
 *  Created on: Aug 15, 2021
 *      Author: huang
 */

#ifndef HW_IIC_HW_IIC_HPP_
#define HW_IIC_HW_IIC_HPP_

#include "main.h"

namespace MyDrivers {

class hw_iic {
public:
    hw_iic();
    void init(I2C_HandleTypeDef *_IIC_Handle,uint8_t _deviceAddress);
    uint8_t readByte(uint8_t regAddress);
    bool readBytes(uint8_t regAddress, uint8_t *pData, uint16_t size);
    bool writeByte(uint8_t regAddress, uint8_t data);
    bool writeBytes(uint8_t regAddress, uint8_t *pdata, uint16_t size);
    void error(void);
    int8_t readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data);
    bool writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data);
    int8_t readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
    bool writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
    virtual ~hw_iic();
private:
    I2C_HandleTypeDef *IIC_Handle;
    uint8_t deviceAddress;
};

} /* namespace MyDrivers */

#endif /* HW_IIC_HW_IIC_HPP_ */
