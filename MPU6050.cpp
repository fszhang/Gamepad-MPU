#include <Arduino.h>
#include "MPU6050.h"
#include <Wire.h>

MPU6050_I2C::MPU6050_I2C(byte chipAddress,TwoWire *TwoWireInstance ) 
{
    _chipAddress = (uint8_t) chipAddress;
    _TwoWireInstance = TwoWireInstance;
} // End constructor

void MPU6050_I2C::WriteRegister( byte reg, byte value ) 
{
    _TwoWireInstance->beginTransmission(_chipAddress);
    _TwoWireInstance->write(reg);
    _TwoWireInstance->write(value);
    _TwoWireInstance->endTransmission();
}

byte MPU6050_I2C::Init( void ) 
{
    byte Data= 0x01; 
    WriteRegister(0x6b,1,&Data);
    Data= 0x00; 
    WriteRegister(0x1b,1,&Data);
    Data= 0x00; 
    WriteRegister(0x1C,1,&Data);    
    return 1;          
}


void MPU6050_I2C::ConvStart(void)
{
    _TwoWireInstance->beginTransmission(_chipAddress);
    _TwoWireInstance->write(0);
    _TwoWireInstance->endTransmission();
}

void MPU6050_I2C::HeatEnable(void)
{
    byte Data[2]; 
    Data[0] = 0x36;
    Data[1] = 0;
    WriteRegister(0x02,2,Data);
}

void MPU6050_I2C::HeatDisable(void)
{
    byte Data[2]; 
    Data[0] = 0x16;
    Data[1] = 0;
    WriteRegister(0x02,2,Data);
}

void MPU6050_I2C::getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{ 
    uint8_t THData[14];
    ReadRegister(0x3B, 14, THData);
    *ax = (((int16_t)THData[0]) << 8) | THData[1];
    *ay = (((int16_t)THData[2]) << 8) | THData[3];
    *az = (((int16_t)THData[4]) << 8) | THData[5];
    *gx = (((int16_t)THData[8]) << 8) | THData[9];
    *gy = (((int16_t)THData[10]) << 8) | THData[11];
    *gz = (((int16_t)THData[12]) << 8) | THData[13];
}



void MPU6050_I2C::WriteRegister( byte reg, byte count, byte *values ) 
{
    if (count == 0) 
        return;
    uint8_t regist = (uint8_t) reg;
    _TwoWireInstance->beginTransmission(_chipAddress);
    _TwoWireInstance->write(regist);
    for (byte index = 0; index < count; index++) 
    {
        _TwoWireInstance->write(values[index]);
    }
    _TwoWireInstance->endTransmission();
}


byte MPU6050_I2C::ReadRegister( byte reg ) 
{
    byte value;
    uint8_t _size = 1;
    uint8_t regist;
    regist = (uint8_t) reg;
    _TwoWireInstance->beginTransmission(_chipAddress);
    _TwoWireInstance->write(regist);
    _TwoWireInstance->endTransmission();
    _TwoWireInstance->requestFrom(_chipAddress, _size);
    value = _TwoWireInstance->read();
    return value;
}

void MPU6050_I2C::ReadRegister( byte count, byte *values ) 
{
    if (count == 0) 
        return;
    uint8_t _count = (uint8_t) count;
    byte index = 0;                    
    _TwoWireInstance->beginTransmission(_chipAddress);  
    _TwoWireInstance->requestFrom(_chipAddress, _count);
    while (_TwoWireInstance->available()) 
    {
        values[index++] = _TwoWireInstance->read();
    }
}

void MPU6050_I2C::ReadRegister( byte reg,  byte count, byte *values ) 
{
    if (count == 0) 
        return;
    uint8_t _count = (uint8_t) count;
    uint8_t regist = (uint8_t) reg;
    byte index = 0;                    
    _TwoWireInstance->beginTransmission(_chipAddress);
    _TwoWireInstance->write(regist);
    _TwoWireInstance->endTransmission();
    _TwoWireInstance->requestFrom(_chipAddress, _count);
    while (_TwoWireInstance->available()) 
    {
        values[index++] = _TwoWireInstance->read();
    }
}
