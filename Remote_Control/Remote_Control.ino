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
Button button3(4, action_set_xbee, (void *)'2'); // blue arcade switch
//Button button2(3, action_light_led, (void *)LED); // blue arcade switch
//Button button3(4, action_light_led, (void *)LED);  // red arcade switch

#define NUM_PINS 14
Pin *pinArray[NUM_PINS] = {
  NULL,     // 0: RX - to Xbee via switch
  NULL,     // 1: TX - to Xbee
  &button1,
  &button2,
  &button3,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
};



void setup() {
  pinMode(LED, OUTPUT);
  pinMode(debugLED, OUTPUT);

  LCD_setup();
  
  Serial.begin(9600);
}

int analogValue = 0;

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
  
  if (Serial.available() >= 23) {
    if (Serial.read() == 0x7E) {
#ifdef DEBUG      
      digitalWrite(debugLED, HIGH);
      debugLEDTime = millis() + 500;
#endif

      for (int i = 0; i < 20; i++) {
        byte discard = Serial.read(); 
      } 
      int analogHigh = Serial.read();
      int analogLow = Serial.read();
      analogValue = analogLow + (analogHigh * 256);
      
      DEBUG_PRINT(2, "Read value ");
      DEBUG_PRINT(2, analogValue);
      DEBUG_PRINT(2, "\n");

      String text = String(String("Xbee value: ") + String(analogValue));
      LCD_set(0, 0, text);
    }
  }
  
  if (checkButtons(pinArray, NUM_PINS)) {
    DEBUG_COMMAND(digitalWrite(debugLED, HIGH));
  } else {
    DEBUG_COMMAND(digitalWrite(debugLED, LOW));
  }

  LCD_loop();
}

