#include <Arduino.h>

#define DEBUG
#define DEBUG_VERBOSE 2

#include "Debug.h"

#include <ShiftBar.h>

#include "LightModuleModes.h"

ShiftBar *Lights;

void light_mode_init(ShiftBar *_SB) 
{
  Lights = _SB;
}

int mode = LIGHT_MODE_RANDOM_FADE; // Starting mode

/* Return the current mode value */
#define MODE_CHANGE_PERIOD (5 * 60 * 1000) // Period between mode changes
int get_current_mode(void) 
{
  static unsigned long nextChangeMillis = millis() + MODE_CHANGE_PERIOD;

  if (millis() > nextChangeMillis) {
    mode = (mode + 1) % LIGHT_MODE_TOTAL;
    nextChangeMillis = millis() + MODE_CHANGE_PERIOD;
  }

  return mode;
}

/*
 * Set the lights to a random color
 *
 * args: period (in ms) to wait between changes
 */
int light_mode_random(void *arg)
{
  for (int i = 0; i < Lights->num_modules; i++) {
    int R = random(0, SHIFTBAR_MAX);
    int G = random(0, SHIFTBAR_MAX);
    int B = random(0, SHIFTBAR_MAX);
    
    Lights->set(i, R, G, B);
    DEBUG_VALUE(2, "R:", R);
    DEBUG_VALUE(2, " G:", G);
    DEBUG_VALUE(2, " B:", B);
  }
  
  return (int)arg;
}

/*
 * Fades up and down between random colors
 */
int light_mode_random_fade(void *arg) 
{
  static float value = 0.0;
  static int direction = 0; // 0 -> Increasing, 1 -> Decreasing

  static float scale = 0.1;

  static int color[3];

  if (value == 0) {
    for (int i = 0; i < 3; i++) {
      color[i] = random(0, SHIFTBAR_MAX);
    }
    scale = random(1, 50)/5000.0;
  }

  if (direction) {
    /* Increasing */
    value += scale;
    if (value >= 1.0) {
      value = 1.0;
      direction = 0;
    }
  } else {
    /* Decreasing */
    value -= scale;
    if (value <= 0) {
      value = 0;
      direction = 1;
    }
  }

  for (int i = 0; i < Lights->num_modules; i++) {
    Lights->set(i, value * color[0], value * color[1], value * color[2]);
  }

  return (int)arg;
}
