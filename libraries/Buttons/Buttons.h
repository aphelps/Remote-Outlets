#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#if (HIGH > 256) || (LOW > 256)
THIS IS A COMPILER ASSERT
#endif

#define DEFAULT_DEBOUNCE_DELAY 50

typedef void (*button_action_t)(int button, int value);

class Button
{
  public:
  Button(byte _pin, button_action_t _action);
  boolean read(void);
  boolean debouncedRead(void);

  button_action_t action;

  private:
  byte pin;
  byte curr_state;
  byte prev_state;

  unsigned long debounce_time;
  unsigned long debounce_delay;
};


boolean checkButtons(Button *buttons, byte num_buttons);

#endif
