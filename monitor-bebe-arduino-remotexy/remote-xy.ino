/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_SOFTSERIAL_POINT

#include <SoftwareSerial.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 10
#define REMOTEXY_SERIAL_TX 11
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_ACCESS_PASSWORD "210424"


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 599 bytes
  { 255,12,0,23,0,80,2,19,0,0,0,77,111,110,105,116,111,114,98,101,
  98,101,0,20,1,106,200,1,1,39,0,130,6,17,95,37,8,16,129,3,
  3,109,10,0,203,240,159,145,182,32,77,111,110,105,116,111,114,32,100,101,
  32,98,101,98,195,169,32,240,159,145,182,0,72,17,27,15,15,12,179,183,
  6,28,0,0,0,0,0,0,200,66,0,0,0,0,129,12,21,23,5,64,
  192,72,117,109,101,100,97,100,0,72,73,27,16,16,4,179,181,2,28,0,
  0,0,0,0,0,200,66,0,0,0,0,129,68,20,30,5,64,2,84,101,
  109,112,101,114,97,116,117,114,97,0,67,17,41,11,8,94,6,30,67,73,
  42,14,8,78,64,30,2,129,30,42,6,7,64,6,37,0,129,88,42,8,
  7,64,2,194,176,67,0,130,1,55,104,45,8,41,129,16,61,80,8,64,
  15,77,111,100,111,115,32,100,101,108,32,99,117,97,114,116,111,32,240,159,
  155,143,239,184,143,0,1,7,73,14,14,0,8,31,0,129,5,89,17,4,
  64,24,65,112,97,103,97,100,111,0,1,27,73,14,14,0,162,31,0,129,
  28,90,13,4,64,162,68,111,114,109,105,114,0,1,49,73,14,14,0,92,
  31,0,129,48,90,15,4,64,77,67,97,109,98,105,111,0,1,69,73,13,
  13,0,27,31,0,129,69,90,14,4,64,27,77,97,110,117,97,108,0,1,
  88,74,12,12,0,1,31,0,129,89,90,12,4,64,1,65,108,101,114,116,
  97,0,130,6,123,48,74,14,180,4,13,140,14,43,32,26,26,10,32,140,
  13,13,48,6,26,31,80,76,65,89,0,1,83,84,79,80,0,1,32,155,
  13,13,0,133,31,78,69,88,84,32,62,62,0,1,32,170,13,13,0,62,
  31,60,60,32,80,82,69,86,0,129,10,138,21,5,64,25,86,111,108,117,
  109,101,110,0,129,11,125,42,9,64,187,83,111,117,110,100,32,240,159,148,
  138,0,67,11,185,37,8,100,6,30,11,129,88,28,16,12,64,17,240,159,
  140,161,239,184,143,0,129,7,30,13,10,64,17,240,159,146,167,0,130,56,
  123,47,74,14,96,129,69,126,27,9,64,215,76,117,122,240,159,146,161,0,
  6,60,144,40,40,202,186,70,20,109,10,10,18,30,37,8,73,61,112,34,
  5,109,133,9,5,26,0,0,0,0,0,0,200,66,0,0,0,0,135,0,
  0,72,66,0,0,140,66,93,0,0,140,66,0,0,180,66,36,0,0,180,
  66,0,0,190,66,129,8,103,34,6,64,1,77,111,118,105,109,105,101,110,
  116,111,0,129,63,102,29,7,64,6,66,97,98,121,32,99,114,121,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t mode_off; // =1 if button pressed, else =0
  uint8_t mode_sleep; // =1 if button pressed, else =0
  uint8_t mode_change; // =1 if button pressed, else =0
  uint8_t mode_manual; // =1 if button pressed, else =0
  uint8_t mode_emergency; // =1 if button pressed, else =0
  int8_t volumen; // from -100 to 100
  uint8_t play_stop; // =1 if state is ON, else =0
  uint8_t next; // =1 if button pressed, else =0
  uint8_t preview; // =1 if button pressed, else =0
  uint8_t rgb_01_r; // =0..255 Red color value
  uint8_t rgb_01_g; // =0..255 Green color value
  uint8_t rgb_01_b; // =0..255 Blue color value

    // output variables
  int8_t humedad; // from 0 to 100
  float temperatura; // from 0 to 100
  int8_t hum_str; // -128 .. 127
  float temp_str;
  char song[11]; // string UTF8 end zero
  uint8_t movimiento; // from 0 to 1
  int8_t cry; // from 0 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_MODE_OFF 2
#define PIN_MODE_SLEEP 3
#define PIN_MODE_CHANGE 8
#define PIN_MODE_MANUAL 9
#define PIN_MODE_EMERGENCY 13


void setup() 
{
  RemoteXY_Init (); 
  
  pinMode (PIN_MODE_OFF, OUTPUT);
  pinMode (PIN_MODE_SLEEP, OUTPUT);
  pinMode (PIN_MODE_CHANGE, OUTPUT);
  pinMode (PIN_MODE_MANUAL, OUTPUT);
  pinMode (PIN_MODE_EMERGENCY, OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_MODE_OFF, (RemoteXY.mode_off==0)?LOW:HIGH);
  digitalWrite(PIN_MODE_SLEEP, (RemoteXY.mode_sleep==0)?LOW:HIGH);
  digitalWrite(PIN_MODE_CHANGE, (RemoteXY.mode_change==0)?LOW:HIGH);
  digitalWrite(PIN_MODE_MANUAL, (RemoteXY.mode_manual==0)?LOW:HIGH);
  digitalWrite(PIN_MODE_EMERGENCY, (RemoteXY.mode_emergency==0)?LOW:HIGH);
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 


}