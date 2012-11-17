/*
 * Code for little lights using ShiftBars
 */

#include <Arduino.h>
#include "ShiftBar.h"

// Analog Values
#define PIN_PHOTORESISTOR 0
#define PIN_DIAL          1

#define NumLEDs 1

uint16_t LEDChannels[NumLEDs][3];

ShiftBar SB(NumLEDs, (uint16_t *)LEDChannels);
 
void setup() {
   Serial.begin(9600);

   /* Initialize the randomseed with the value of an unconnected analog pin */
   randomSeed(analogRead(5));
}
 
#define MODES 7

#define VALUE_INC 1

#define DELAY 10

/* Threshold value for the photoresistor */
#define THRESHOLD_HIGH 670
#define THRESHOLD_LOW  570

void LED_set_mode1(int cycle, int value) {
  switch (cycle % MODES) {
    case 0: { // All on
      LEDChannels[0][0] = value;
      LEDChannels[0][1] = value;
      LEDChannels[0][2] = value;      
      break;
    }
    case 1: {  // Red
      LEDChannels[0][SHIFTBAR_RED] = value;
      break;
    }
    case 2: { // Green
      LEDChannels[0][SHIFTBAR_GREEN] = value;
      break;     
    }
    case 3: {  // Blue
      LEDChannels[0][SHIFTBAR_BLUE] = value;
      break;
    }
    case 4: { // Red and Green
      LEDChannels[0][SHIFTBAR_RED] = value;
      LEDChannels[0][SHIFTBAR_GREEN] = value;
      break;
    }
    case 5: {
      LEDChannels[0][SHIFTBAR_GREEN] = value;
      LEDChannels[0][SHIFTBAR_BLUE] = value;      
      break;
    }
    case 6: {
      LEDChannels[0][SHIFTBAR_RED] = value;
      LEDChannels[0][SHIFTBAR_BLUE] = value;      
      break;
    }
  }  
}

void LED_set_random(int cycle, int value) {
  static int last_cycle = -1;
  static double color[3];

  if (cycle != last_cycle) {
    double max = 0;
    for (int i = 0; i < 3; i++) {
      color[i] = random(0, SHIFTBAR_MAX);
      if (color[i] > max) max = color[i];
    }
    
    for (int i = 0; i < 3; i++) {
      color[i] = color[i] / max; 
    }
    
    last_cycle = cycle;
  }
  
  for (int i = 0; i < 3; i++) {
    LEDChannels[0][i] = value * color[i];
  }      
}

void loop() {
  static int cycle;
  static int value;
  static int direction = 1;
  static boolean lights_on = false;

  LEDChannels[0][0] = 0;
  LEDChannels[0][1] = 0;
  LEDChannels[0][2] = 0;
  
//  int lightvalue = THRESHOLD + 1;
  int lightvalue = analogRead(PIN_PHOTORESISTOR);
  if (lightvalue > THRESHOLD_HIGH) {
    lights_on = true;
  } else if (lightvalue < THRESHOLD_LOW) {
    lights_on = false;
  }
  
#ifdef DIAL
  int dialvalue = analogRead(PIN_DIAL);
  mode = map(dialvalue, 250, 870, 0, MODES);
  if (mode == MODES) {
    mode = cycle % MODES;
  }
#endif   

  Serial.print("light ");
  Serial.print(lightvalue);
#ifdef DIAL
  Serial.print("dial ");
  Serial.print(dialvalue);
#endif
  Serial.print(" value ");
  Serial.print(value);
  Serial.print(" cycle ");
  Serial.print(cycle);
  Serial.print("\n"); 
  
  if (lights_on) {    
    if (direction)  {
      value = (value + VALUE_INC);
      if (value > SHIFTBAR_MAX) {
        value = SHIFTBAR_MAX;  
        //cycle++;
        direction = 0;
      }
    } else {
      value = (value - VALUE_INC);
      if (value <= 0) {
        value = 0;
        cycle++;
        direction = 1;       
      }
    }
    
    //  LED_set_mode1(cycle, value);
    LED_set_random(cycle, value);
  } else {
    LEDChannels[0][0] = 0;
    LEDChannels[0][1] = 0;
    LEDChannels[0][2] = 0;
  }
  
  // Send values to LEDs
  SB.update();
  
  delay(DELAY);
}

