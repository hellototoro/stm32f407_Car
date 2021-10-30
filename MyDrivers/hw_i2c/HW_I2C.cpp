/*
 * hwiic.cpp
 *
 *  Created on: Aug 15, 2021
 *      Author: huang
 */

#include <hw_i2c/HW_I2C.hpp>

namespace MyDrivers {

#define I2C_TIMEOUT  1000 /*<! Value of Timeout when I2C communication fails */

HW_I2C::HW_I2C() {
    // TODO Auto-generated constructor stub

}

void HW_I2C::init(I2C_HandleTypeDef *IIC_Handle,uint8_t deviceAddress)
{
    this->IIC_Handle = IIC_Handle;
    this->deviceAddress = deviceAddress;
}

void HW_I2C::error(void)
{
    /* De-initialize the IOE comunication BUS */
    //HAL_I2C_DeInit(I2C_Handle);

    /* Re-Initiaize the IOE comunication BUS */
    //I2Cx_Init();
    HAL_I2C_MspInit(IIC_Handle);
    HAL_I2C_Init(IIC_Handle);
}

uint8_t HW_I2C::readByte(uint8_t regAddress)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t Value = 0;

    status = HAL_I2C_Mem_Read(IIC_Handle, deviceAddress, regAddress, I2C_MEMADD_SIZE_8BIT, &Value, 1, I2C_TIMEOUT);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        error();
    }

    return Value;
}

bool HW_I2C::readBytes(uint8_t regAddress, uint8_t *pData, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Read(IIC_Handle, deviceAddress, regAddress, I2C_MEMADD_SIZE_8BIT, pData, size, I2C_TIMEOUT);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        error();
    }

    return static_cast<bool>(status);
}

bool HW_I2C::writeByte(uint8_t regAddress, uint8_t data)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(IIC_Handle, deviceAddress, (uint16_t)regAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* I2C error occured */
        error();
    }

    return static_cast<bool>(status);
}

bool HW_I2C::writeBytes(uint8_t regAddress, uint8_t *pdata, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(IIC_Handle, deviceAddress, (uint16_t)regAddress, I2C_MEMADD_SIZE_8BIT, pdata, size, I2C_TIMEOUT);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Re-Initiaize the I2C Bus */
        error();
    }

    return static_cast<bool>(status);
}

/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t HW_I2C::readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data)
{
    uint8_t count = 0xff;
    uint8_t b = readByte(regAddr);
    *data = b & (1 << bitNum);
    return count;
}

/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool HW_I2C::writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t b = readByte(regAddr);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(regAddr, b);
}
/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */

int8_t HW_I2C::readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count = 0xff;
    uint8_t b = readByte(regAddr);
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
    /*if ((count = readByte(regAddr, &b)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }*/
    return count;
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
bool HW_I2C::writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b = readByte(regAddr);
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte
    return writeByte(regAddr, b);
    /*if (readByte(devAddr, regAddr, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return writeByte(devAddr, regAddr, b);
    } else {
        return false;
    }*/
}

HW_I2C::~HW_I2C() {
    // TODO Auto-generated destructor stub
}

} /* namespace MyDrivers */
