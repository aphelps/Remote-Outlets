/*
 * Button class
 *
 * Written by Adam Phelps (amp@cs.stanford.edu)
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define DEFAULT_DEBOUNCE_DELAY 50

typedef enum 
{
  PIN_TYPE_NONE,
  PIN_TYPE_BUTTON
} pin_type_t;
  

class Pin 
{
  public:
  Pin(byte _pin, boolean _analog, pin_type_t _type);
  Pin(byte _pin, boolean _analog);

  pin_type_t type;
  boolean analog;

  protected:
  byte pin;
};

typedef void (*button_action_t)(int button, int value, void *arg);

class Button : public Pin
{
  public:
  Button(byte _pin, boolean _analog, button_action_t _action);
  Button(byte _pin, boolean _analog,
         button_action_t _action, void *_action_arg);
  int read(void);
  int debouncedRead(void);

  button_action_t action;
  void *action_arg;

  private:
  void init(byte _pin, button_action_t _action);

  unsigned int curr_state;
  unsigned int prev_state;

  unsigned long debounce_time;
  unsigned long debounce_delay;
};


boolean checkButtons(Pin **pins, byte num_pins, boolean debounce);

#endif
