/*
 * Remote Control code
 *
 * Written by Adam Phelps (amp@cs.stanford.edu)
 */

#include <LiquidCrystal.h>

#include "Buttons.h"
#include "Debug.h"
#include "LCD.h"

int LED = 11;
int debugLED = 12;


Button button1(2, NULL);
Button button2(3, action_set_xbee, (void *)'1'); // blue arcade switch
Button button3(4, action_set_xbee, (void *)'2'); // red arcade switch
//Button button2(3, action_light_led, (void *)LED); // blue arcade switch
//Button button3(4, action_light_led, (void *)LED);  // red arcade switch

#define NUM_PINS 14
Pin *pinArray[NUM_PINS] = {
  NULL,     // 0: RX - to Xbee via switch
  NULL,     // 1: TX - to Xbee
  &button1, // 2: on-board momentary
  &button2, // 3: blue arcade button
  &button3, // 4: red arcade button
  NULL,     // 5: LCD
  NULL,     // 6: LCD
  NULL,     // 7: LCD
  NULL,     // 8: LCD
  NULL,     // 9: LCD
  NULL,     // 10: LCD
  NULL,     // 11: LED
  NULL,     // 12: LED
  NULL,     // 13: Empty
};



void setup() {
  pinMode(LED, OUTPUT);
  pinMode(debugLED, OUTPUT);

  LCD_setup();
  
  Serial.begin(9600);
}

#ifdef DEBUG
unsigned long debugLEDTime = 0;
#endif

void loop() {
#ifdef DEBUG
  if (debugLEDTime && (millis() > debugLEDTime)) {
    debugLEDTime = 0;
    digitalWrite(debugLED, LOW);
  }
#endif
  
  
  if (checkButtons(pinArray, NUM_PINS)) {
    DEBUG_COMMAND(digitalWrite(debugLED, HIGH));
  } else {
    DEBUG_COMMAND(digitalWrite(debugLED, LOW));
  }

  LCD_loop();

  delay(10);
}

