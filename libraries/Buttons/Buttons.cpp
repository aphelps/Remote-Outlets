/*
 * Code for on-off controls
 */
#include <Arduino.h>

#include "Buttons.h"
#include "Debug.h"

Button::Button(byte _pin, button_action_t _action) 
{
  pin = _pin;
  pinMode(pin, OUTPUT);
  action = _action;

  /* Init to defaults */
  curr_state = LOW;
  prev_state = LOW;
  debounce_time = 0;
  debounce_delay = DEFAULT_DEBOUNCE_DELAY;
}

boolean
Button::read(void) 
{
  curr_state = digitalRead(pin);
  return (curr_state == HIGH);
}


boolean
Button::debouncedRead(void)
{
  byte currentValue = digitalRead(pin);
  
  if (currentValue != prev_state) {
    debounce_time = millis();
    DEBUG_PRINT(2, "Button ");
    DEBUG_PRINT(2, pin);
    DEBUG_PRINT(2, ": buttons_debounce_time=");
    DEBUG_PRINT(2, debounce_time);
    DEBUG_PRINT(2, "\n");
  }
  prev_state = currentValue;
  
  if (currentValue != curr_state) {
    if ((millis() - debounce_time) > debounce_delay) {
      curr_state = currentValue;
      DEBUG_PRINT(2, "Button ");
      DEBUG_PRINT(2, pin);
      DEBUG_PRINT(2, ": value=");
      DEBUG_PRINT(2, curr_state);
      DEBUG_PRINT(2, "\n");

      if (action) action(pin, currentValue);
    }
  }
  
  return (curr_state == HIGH);
}


boolean
checkButtons(Button *buttons, byte num_buttons) {
  for (byte i = 0; i < num_buttons; i++) {
    if (buttons[i].debouncedRead())
      return true;
  }
  return false;
}
