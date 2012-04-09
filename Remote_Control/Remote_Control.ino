/*
 * Remote Control code
 *
 * Written by Adam Phelps (amp@cs.stanford.edu)
 */

#include <LiquidCrystal.h>

#include "Buttons.h"
#include "Debug.h"
#include "LCD.h"
#include "Menu.h"

int LED = 11;
int debugLED = 12;

MenuItem menu1("Sample Action", "Counter", NULL, NULL);
MenuItem menu2("Toggle Xbee 1", "Toggle", menu_toggle_xbee, (void *)'1');
MenuItem menu3("Toggle Xbee 2", "Toggle", menu_toggle_xbee, (void *)'2');
MenuItem menu4("Menu Line 4", "Action 4", NULL, NULL);

#define NUM_MENU_ITEMS 4
MenuItem *menuArray[NUM_MENU_ITEMS] = {
  &menu1,
  &menu2,
  &menu3,
  &menu4
};
Menu menu(NUM_MENU_ITEMS, menuArray, &lcd);


Button button1(2, NULL);
//Button button2(3, button_set_xbee, (void *)'1'); // blue arcade switch
//Button button3(4, button_set_xbee, (void *)'2'); // red arcade switch
Button button2(3, button_light_led, (void *)LED); // blue arcade switch
Button button3(4, button_light_led, (void *)LED);  // red arcade switch
//Button button2(3, button_set_lcd, NULL); // blue arcade switch
//Button button3(4, button_set_lcd, NULL);  // red arcade switch
//Button button2(3, button_menu_next, &menu); // blue arcade switch
//Button button3(4, button_menu_select, &menu);  // red arcade switch

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
#endif

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(debugLED, OUTPUT);

  
//  LCD_setup();
//  LCD_set(0, 0, "This is a test", true);
//  menu.display();
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

