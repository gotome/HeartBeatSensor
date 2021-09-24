///////////////////////////////////////////
//                LIBRARYS
///////////////////////////////////////////

// G E N E R A L 
#include <stdlib.h>
// A R D U I N O
#include <Arduino.h>
#include <math.h>
// D I S P L A Y
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// H E A R T  R A T E  S E N S O R
#include <DFRobot_Heartrate.h>
// P R O J E C T
#include "HeartBeatSensor.h"
 
///////////////////////////////////////////
//                GLOBALS
///////////////////////////////////////////
const int i16BuzzerPin = DI_PIN_9;

// calculated heart rate from heart rate sensor
int i16HeartRate = 0;

// raw heart rate analog value
int i16SensorVal = 0;

// coordinates for electro cardiogram
int i16OldX = 0; 
int i16X = 0; 
int i16OldY = 0; 
int i16Y = 0; 

// tone timer (milliseconds)
long l32PrevToneTime=0;
long l32CurrToneTime=0;

// time until the next tone is played 
const long l32EventIntervall = 1000;

// the tempo is how fast to play the song.
// to make the song play faster, decrease this value.
const long l32ToneDuration = 150;

// Frequency of the alarm tone
const int i16ToneFrequency = 1000;

///////////////////////////////////////////
//         FUNCTION DECLARATIONS
///////////////////////////////////////////
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DFRobot_Heartrate heartrate(DIGITAL_MODE); ///< ANALOG_MODE or DIGITAL_MODE

///////////////////////////////////////////
//         FUNCTION DEFINITIONS
///////////////////////////////////////////

void playTone(void) {
  // play tone each second
  l32CurrToneTime = millis();

  if ((l32CurrToneTime - l32PrevToneTime) >= l32EventIntervall) {
    tone(i16BuzzerPin, i16ToneFrequency, l32ToneDuration);
    l32PrevToneTime = l32CurrToneTime;
  }
}


void showStats(int heartRate, int i16SensorVal) 
{ 
  char charVal[5]; 
  dtostrf(heartRate, 4, 0, charVal);

  //reset cursor if end of display is reached
  if (i16X > 127) {
    display.clearDisplay(); 
    i16X = 0;
    i16OldX = i16X; 
  }

  i16Y = 60-(i16SensorVal/16);
  i16OldX = i16X; 
  i16OldY = i16Y; 

  //show cardiogramm
  display.writeLine(i16OldX, i16OldY, i16X, i16Y, WHITE);

  //show beats per minute
  display.writeFillRect(0,50,128,16,BLACK);
  display.setCursor(10, 50);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(charVal);
  display.print(" BPM");
  display.display();

  //finally increase ECG x-coordinate
  i16X++;
}

///////////////////////////////////////////
//              S E T U P
//////////////////////////////////////////
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
  pinMode(i16BuzzerPin, OUTPUT);
  //delay time
  delay(10);
}

///////////////////////////////////////////
//              M A I N
//////////////////////////////////////////
void loop() 
{  
  i16SensorVal = 700; //heartrate.getValue(A0);
  i16HeartRate = 60; //heartrate.getRate();
  //Serial output
  Serial.print("heart rate: " + String(i16HeartRate) + "\t");
  Serial.println("sensor value: " + String(i16SensorVal) + "\t");
  Serial.println("x-coordinate: " + String(i16X) + "\t");

  //show statistics on lcd terminal  
  showStats(i16HeartRate, i16SensorVal);

  //play alarm if heart rate is to low
  if (i16HeartRate < 60) {
    playTone();
  }  
}
