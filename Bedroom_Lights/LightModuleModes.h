#ifndef LIGHTMODULEMODES
#define LIGHTMODULEMODES

#include "ShiftBar.h"

/* Modes for the lights */
#define LIGHT_MODE_RANDOM      0
#define LIGHT_MODE_RANDOM_FADE 1
#define LIGHT_MODE_TOTAL       2

/*
 * Definition of light mode function
 *
 * Return: period (in ms) to wait between mode calls
 */
typedef int (*light_mode_function_t)(void *arg);

/* Structure containing both a light mode function and its arguments */
typedef struct {
  light_mode_function_t function;
  void *arg;
} light_pair_t;

/* Mode functions */
int light_mode_random(void *arg);
int light_mode_random_fade(void *arg);

/* Initialize the light mode module */
void light_mode_init(ShiftBar *SB);

/* Return the current mode value */
int get_current_mode(void);

#endif
