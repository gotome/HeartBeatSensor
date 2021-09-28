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

// calculated heart rate from heart rate sensor
unsigned int u16HeartRate = 0;

// raw heart rate analog value
unsigned int u16SensorVal = 0;

// coordinates for electro cardiogram
int i16OldX = 0; 
int i16X = 0; 
int i16OldY = 0; 
int i16Y = 0; 

// tone timer (milliseconds)
long l32PrevToneTime=0;
long l32CurrToneTime=0;

//valid heart rate timer (milliseconds)
long l32PrevBpmTime=0;
long l32CurrBpmTime=0;

//last valid heart rate
unsigned int u16LastValBpm = 0;

// time until the next tone is played 
const long l32EventIntervallBpm = 5000;

// time until the next tone is played 
const long l32EventIntervallTone = 1000;

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

  if ((l32CurrToneTime - l32PrevToneTime) >= l32EventIntervallTone) {
    tone(DI_PIN_9, i16ToneFrequency, l32ToneDuration);
    l32PrevToneTime = l32CurrToneTime;
  }
}


void showStats(unsigned int heartRate) 
{ 
  //reset cursor if end of display is reached
  if (i16X > 127) {
    display.clearDisplay(); 
    i16X = 0;
    i16OldX = i16X; 
  }

  if (heartRate == 0) {
    i16Y = i16OldY; 
  } else {
    i16Y = 45 - ((heartRate / 5) % 45);
  }
  i16OldX = i16X; 
  i16OldY = i16Y; 

  //show cardiogramm
  display.writeLine(i16OldX, i16OldY, i16X, i16Y, WHITE);

  //show beats per minute
  display.writeFillRect(0,50,128,16,BLACK);
  display.setCursor(10, 50);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(String(heartRate));  
  display.print(" BPM");  
  display.display();

  //finally increase ECG x-coordinate
  i16X++;
}

unsigned int getDebouncedHeartRate(unsigned int heartRate) {
  l32CurrBpmTime = millis();
  
  if (heartRate > 0) {
    u16LastValBpm = heartRate; 
    l32PrevBpmTime = l32CurrBpmTime;
  }
  else {
    if ((l32CurrBpmTime - l32PrevBpmTime) >= l32EventIntervallBpm) {
      u16LastValBpm = 0;
    }
  }
  return u16LastValBpm;
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
    while(true){} // don´t proceed, loop forever
  } 
  display.clearDisplay();  

  //pin mode setup 
  pinMode(DI_PIN_9, OUTPUT);
}

///////////////////////////////////////////
//              M A I N
//////////////////////////////////////////
void loop() 
{  
  u16SensorVal = heartrate.getValue(AI_PIN_0);
  u16HeartRate = heartrate.getRate();
  u16HeartRate = getDebouncedHeartRate(u16HeartRate);

  // serial output
  if (u16HeartRate) {
    Serial.print("heart rate: " + String(u16HeartRate) + "\t");
    Serial.println("sensor value: " + String(u16SensorVal) + "\t");
    //Serial.println("x-coordinate: " + String(i16X) + "\t");
  }

  // show statistics on lcd terminal    
  showStats(u16HeartRate);

  // play alarm if heart rate is to low
  if (u16HeartRate < 60 || u16HeartRate > 190) {
    playTone();
  }  

  delay(20);

}
