/*
 * Remote Control code
 *
 * Written by Adam Phelps (amp@cs.stanford.edu)
 */

#include <LiquidCrystal.h>

//#define DEBUG
//#define DEBUG_VERBOSE 2

#include "Pins.h"
#include "Debug.h"
#include "LCD.h"
#include "Menu.h"

int LED = 11;
int debugLED = 12;

MenuItem menu_xbee1("Toggle Xbee 1", "Toggle XBee1",
                    menu_toggle_xbee, (void *)'1');
MenuItem menu_xbee2("Toggle Xbee 2", "Toggle XBee2",
                    menu_toggle_xbee, (void *)'2');
MenuItem menu_example("Sample Action", "Counter",
                      menu_count, NULL);

#define NUM_MENU_ITEMS 3
MenuItem *menuArray[NUM_MENU_ITEMS] = {
  &menu_xbee1,
  &menu_xbee2,
  &menu_example,
};
Menu menu(NUM_MENU_ITEMS, menuArray, &lcd);



//Switch switch2(3, switch_set_xbee, (void *)'1'); // blue arcade switch
//Switch switch3(4, switch_set_xbee, (void *)'2'); // red arcade switch
//Switch switch2(3, switch_light_led, (void *)LED); // blue arcade switch
//Switch switch3(4, switch_light_led, (void *)LED);  // red arcade switch
//Switch switch2(3, switch_set_lcd, NULL); // blue arcade switch
//Switch switch3(4, switch_set_lcd, NULL);  // red arcade switch
Switch arcade_blue(2, false, switch_menu_next, &menu); // blue arcade switch
Switch arcade_red(3, false, switch_menu_select, &menu);  // red arcade switch

Switch joystick_vert(5, true, joystick_menu_vert, &menu); // Joystick vertical
Switch joystick_horz(6, true, joystick_menu_horz, &menu); // Joystick vertical
Switch joystick_switch(7, true, joystick_menu_select, &menu); // Joystick vertical

#define NUM_PINS 21
Pin *pinArray[NUM_PINS] = {
  /* Digital Pins */
  NULL,             // D0: RX - to Xbee via switch
  NULL,             // D1: TX - to Xbee
  &arcade_blue,     // D2: blue arcade switch
  &arcade_red,      // D3: red arcade switch
  NULL,             // D4: empty
  NULL,             // D5: LCD
  NULL,             // D6: LCD
  NULL,             // D7: LCD
  NULL,             // D8: LCD
  NULL,             // D9: LCD
  NULL,             // D10: LCD
  NULL,             // D11: LED
  NULL,             // D12: LED
  NULL,             // D13: Empty

  /* Analog Pins */
  NULL,             // A1: Empty
  NULL,             // A2: Empty
  NULL,             // A3: Empty
  NULL,             // A4: Empty
  &joystick_vert,   // A5: Joystick
  &joystick_horz,   // A6: Joystick
  &joystick_switch, // A7: Joystick
};

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(debugLED, OUTPUT);

  
  LCD_setup();
  menu.display();
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
  
  if (checkSwitches(pinArray, NUM_PINS, false)) {
    DEBUG_COMMAND(digitalWrite(debugLED, HIGH));
  } else {
    DEBUG_COMMAND(digitalWrite(debugLED, LOW));
  }

  LCD_loop();

  delay(10);
}

