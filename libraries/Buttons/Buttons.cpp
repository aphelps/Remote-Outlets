/*
 * Code for on-off controls
 */
#include <Arduino.h>

#include "Buttons.h"
#include "Debug.h"

Pin::Pin(byte _pin, pin_type_t _type)
{
  pin = _pin;
  type = _type;
}

Pin::Pin(byte _pin)
{
  Pin(_pin, PIN_TYPE_NONE);
}

void
Button::init(byte _pin, button_action_t _action)
{
  pinMode(pin, OUTPUT);
  action = _action;

  /* Init to defaults */
  curr_state = LOW;
  prev_state = LOW;
  debounce_time = 0;
  debounce_delay = DEFAULT_DEBOUNCE_DELAY;
}


Button::Button(byte _pin, button_action_t _action) 
    : Pin(_pin, PIN_TYPE_BUTTON)
{
  init(_pin, _action);
  action_arg = NULL;

}

Button::Button(byte _pin, button_action_t _action, void *_action_arg)
    : Pin(_pin, PIN_TYPE_BUTTON)
{
  init(_pin, _action);
  action_arg = _action_arg;
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

      if (action) action(pin, currentValue, action_arg);
    }
  }
  
  return (curr_state == HIGH);
}

/* Check the state of every sensor */
boolean
checkButtons(Pin **pins, byte num_pins) {
  boolean retval = false;
  for (byte i = 0; i < num_pins; i++) {
    Pin *pin = pins[i];
    if (pin->type == PIN_TYPE_BUTTON)
      if (((Button *)pin)->debouncedRead())
        retval = true;
  }
  return retval;
}
