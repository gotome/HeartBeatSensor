//****************************************
//                LIBRARYS
//****************************************
//general
#include <stdlib.h>
//arduino
#include <Arduino.h>
#include <math.h>
//display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//project
#include "HeartBeatSensor.h"
 
//****************************************
//                TABLES
//****************************************


//****************************************
//                GLOBALS
//****************************************


//****************************************
//         FUNCTION DECLARATIONS
//****************************************
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//****************************************
//         FUNCTION DEFINITIONS
//****************************************


//setup system and bus communication
void setup()
{
  //begin serial communication
  Serial.begin(115200);
  //display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){ //initialize with the I2C addr 0x3C (128x64)
    Serial.println("Display (SSD1306) allocation failed");
  } 
  display.clearDisplay();
  //pin mode setup 
  pinMode(8, OUTPUT);
  //delay time
  delay(10);
}

//****************************************
//                M A I N
//****************************************
void loop()
{
  
  //delay
  delay(10);
}
