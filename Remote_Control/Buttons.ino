/*
 * Code for on-off controls
 */

typedef void (*button_action_t)(int button, int value);

#define MAX_BUTTONS 4
int num_buttons = 0;
int buttons_pin[MAX_BUTTONS];
int buttons_curr_state[MAX_BUTTONS];
int buttons_prev_state[MAX_BUTTONS];
long buttons_debounce_time[MAX_BUTTONS];
button_action_t buttons_action[MAX_BUTTONS];

void action_set_lcd(int button, int value) 
{
  String text = String(String("button ") + String(button) + String(":") + String(value));
  LCD_set(button - 1, 0, text);
}

void action_light_led(int button, int value) 
{
  digitalWrite(LED, value);
}

void buttons_set_action(int button, void (*action)(int button, int value)) 
{
  buttons_action[button] = action;
}


void buttons_init(const int buttons[], const int nbuttons) 
{
  num_buttons = nbuttons;
  for (int i = 0; i < num_buttons; i++) {
    buttons_pin[i] = buttons[i];

    /* Set the pin for this button */
    pinMode(buttons_pin[i], INPUT);

    /* Initialize debounce states */
    buttons_curr_state[i] = LOW;
    buttons_prev_state[i] = LOW;
    buttons_debounce_time[i] = 0;

    /* Initialize button action */
    //buttons_action[i] = action_set_lcd;
    buttons_set_action(i, action_set_lcd);
  }
}

  
int debounced_read(int button, int debounce_delay) {
  int currentValue = digitalRead(buttons_pin[button]);
  
  if (currentValue != buttons_prev_state[button]) {
    buttons_debounce_time[button] = millis();
    DEBUG_PRINT(2, "Button ");
    DEBUG_PRINT(2, button);
    DEBUG_PRINT(2, ": buttons_debounce_time=");
    DEBUG_PRINT(2, buttons_debounce_time[button]);
    DEBUG_PRINT(2, "\n");
  }
  buttons_prev_state[button] = currentValue;
  
  if (currentValue != buttons_curr_state[button]) {
    if ((millis() - buttons_debounce_time[button]) > debounce_delay) {
      buttons_curr_state[button] = currentValue;
      DEBUG_PRINT(2, "Button ");
      DEBUG_PRINT(2, button);
      DEBUG_PRINT(2, ": value=");
      DEBUG_PRINT(2, buttons_curr_state[button]);
      DEBUG_PRINT(2, "\n");

      if (buttons_action[button]) buttons_action[button](button, currentValue);
    }
  }
  
  return buttons_curr_state[button];
}


boolean checkButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (debounced_read(i, 50) == HIGH)
      return true;
  }
  return false;
}
