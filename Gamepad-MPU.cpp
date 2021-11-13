#include "Arduino.h"
#include "MPU6050.h"
#include <Wire.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include "BleGamepad.h"
#include "GamepadCtrl.h"
#include <string.h>

#define LEDS_COUNT  7
#define LEDS_PIN    12
#define CHANNEL     0
#define BTN_PIN_S   25
#define BT_BTS      6
#define BT_BTW    4
#define BT_BTN    5


#define SAMPLE_COUNT    10


MPU6050_I2C mpu6050(0x68);   // Create MFRC522 instance.
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);


int16_t temperature;
byte sensor;
int16_t ax, ay, az;
int16_t gx, gy, gz;
uint16_t voltage;
GamepadCtrl gamepadCtrl;
uint8_t PowerUp = 0;
uint16_t PowerUpIndex;
uint8_t RxReFlag;
uint16_t buttonData;
uint8_t AX,AY;
uint16_t AX_SUM,AY_SUM;

void setup() 
{
    Serial.begin(115200);           // Initialize serial communications with the PC
    Wire.begin();                   // Initialize I2C
    sensor = mpu6050.Init();
    Serial.println("Sensor Ready!"); 
    pinMode(34,INPUT);

    Serial.println("Starting BLE work!");
    bleGamepad.begin();
    RxReFlag = 0;   //Receive ready   

    strip.begin();
    strip.setBrightness(100);
    pinMode(BTN_PIN_S,INPUT_PULLUP);   
}

void SetLEDColor(byte R, byte G, byte B)
{
    
    for (int i = 0; i < LEDS_COUNT; i++) 
    {
        strip.setLedColorData(i, R, G, B);               
    } 
    strip.show();
    delay(100);   
}
 
void loop() 
{
    AX_SUM = 0;
    AY_SUM = 0;
    for(byte i=0;i<SAMPLE_COUNT;i++)
    {
        mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        if(ax<- 9780)
            ax = - 9780;
        else if(ax > 10220)
            ax = 10220;
        ax += 9780;       

        if(ay<-10000)
            ay = -10000;
        else if(ay>10000)
            ay = 10000;          
        ay = 10000 - ay;

        AX_SUM += (ay/79)&0xff;
        AY_SUM += (ax/79)&0xff;

    }
    AX = AX_SUM/SAMPLE_COUNT;
    AY = AY_SUM/SAMPLE_COUNT;
    //voltage = analogRead(34)*43/25;
    voltage = analogRead(34);
    if(voltage>2440)
        voltage = 2440;
    else if(voltage<1932)
        voltage = 1932;
    voltage = (voltage - 1930)/2; 
    buttonData = ((~digitalRead(BTN_PIN_S))&0x01)<<BT_BTN;
    
    if (gamepadDataGet == 1)
    {
        Serial.println("DataReceived:");
        if( (gamepadOutData[0]==0x0A) && (gamepadOutData[1]==0x0B) )
        SetLEDColor(gamepadOutData[2],gamepadOutData[3],gamepadOutData[4]);
        gamepadDataGet = 0;
    }

    if (bleGamepad.isConnected())
    {
        if (PowerUp == 0)  //Power up first time
        {
            SetLEDColor(0,0,255);
            PowerUpIndex++;
            delay(200);
            if (PowerUpIndex > 15) //Power up for a while 
                PowerUp = 1;
        }//PowerUp==0
        else    //Power up for a while, than can send data
        {
            byte vData[10];
            vData[0] = voltage&0xff;
            for(byte i=1; i< 10; i++)
                vData[i] = 0;
            gamepadCtrl.setCustomData(vData, 10);
            gamepadCtrl.setCtrlData(
                buttonData,      //Buttons, 16bit
                AX,   //X
                AY,   //Y
                127,   //Z
                0,   //Rx
                0,   //Ry
                127);  //Rz
            gamepadCtrl.sendReport();
            //delay(20);                                  
        }//PowerUp for a while can send data
    }//Connected  
    else
    {
        SetLEDColor(0,0,0);
    }               
}
