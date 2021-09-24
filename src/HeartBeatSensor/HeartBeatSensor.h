#pragma once 
//***************************************************************
//                      TYPEDEFINITIONS
//***************************************************************

//***************************************************************
//                      DEFINE MACROS
//***************************************************************
// DISPLAY
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)

//ANALOG PINS
#define DO_PIN_A0 A0

//DIGITAL PINS
#define DI_PIN_8 8
#define DI_PIN_9 9
#define DI_PIN_10 10
#define DI_PIN_11 12
#define DI_PIN_13 13

//***************************************************************
//                     FUNCTION PROTOTYPES
//***************************************************************
void playTone(void);
int frequency(char note);
void showStats(int heartRate);