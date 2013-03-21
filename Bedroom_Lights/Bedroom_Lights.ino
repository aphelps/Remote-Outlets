/*
 * Code for little lights using ShiftBars
 */

#include <Arduino.h>

//#define DEBUG
//#define DEBUG_VERBOSE 2

#include "Debug.h"
#include "LightModuleModes.h"
#include "ShiftBar.h"

// Analog Values
#define PIN_PHOTORESISTOR 0

uint16_t values[1][3];

#define UNLIT_DELAY 5000 // Period to wait when checking photoresister

ShiftBar SB(1, (uint16_t *)values);

light_pair_t modeFunctions[] = {
  { light_mode_random, (void *)1000 },  // LIGHT_MODE_RANDOM
  { light_mode_random_fade, (void *)10 },  // LIGHT_MODE_RANDOM
};

void setup() {
   Serial.begin(9600);

   /* Initialize the randomseed with the value of an unconnected analog pin */
   randomSeed(analogRead(5));

   /* Initialize the light mode module */
   light_mode_init(&SB);
}
 
#define MODES 7

#define VALUE_INC 1

#define DELAY 10

/* Threshold value for the photoresistor */
#define THRESHOLD_HIGH 650
#define THRESHOLD_LOW  500

void loop() {
  static boolean lights_on = false;
  unsigned long delay_period;

  /* Determine if the lights should be on */
  int lightvalue = analogRead(PIN_PHOTORESISTOR);
  if (lightvalue > THRESHOLD_HIGH) {
    lights_on = true;
  } else if (lightvalue < THRESHOLD_LOW) {
    lights_on = false;
  }
  
  DEBUG_VALUE(2, "light:", lightvalue);
  DEBUG_VALUE(2, " on:", lights_on);

  if (lights_on) {
    /* Call the function to set the lights */
    int mode = get_current_mode();
    DEBUG_VALUE(2, " mode:", mode);

    delay_period = modeFunctions[mode].function(modeFunctions[mode].arg);
  } else {
    SB.set(0, 0, 0);
    delay_period = UNLIT_DELAY;
  }
  SB.update();

  DEBUG_VALUELN(2, " delay:", delay_period);

  /* Wait for the specified interval */
  delay(delay_period);
}
