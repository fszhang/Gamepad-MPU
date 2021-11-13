#ifndef GAMEPADCTRL_H
#define GAMEPADCTRL_H

#include "BleGamepad.h"
#include "sdkconfig.h"

class GamepadCtrl 
{
private:
 
    uint16_t _buttons;
    uint8_t _x;
    uint8_t _y;
    uint8_t _z;
    uint8_t _rX;
    uint8_t _rY;
    uint8_t _rZ;
    uint8_t _customData[10];
public:
    void setCustomData(uint8_t* cData, uint8_t dataLen);
    void setCtrlData(uint16_t b = 0,uint8_t x=0, uint8_t y=0, uint8_t z=0, uint8_t rX=0, uint8_t rY=0, uint8_t rZ=0);
    void sendReport(void);
    
};

#endif // GAMEPADCTRL_H
