int buttonState[NUM_BUTTONS];
int lastState[NUM_BUTTONS];
long debounceTime[NUM_BUTTONS];

void debounce_init() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonState[i] = LOW;
    lastState[i] = LOW;
    debounceTime[i] = 0;
  }
}
  
int debounced_read(int button, int debounce_delay) {
  int currentValue = digitalRead(BUTTONS[button]);
  
  if (currentValue != lastState[button]) {
    debounceTime[button] = millis();
    DEBUG_PRINT(2, "Button ");
    DEBUG_PRINT(2, button);
    DEBUG_PRINT(2, ": debounceTime=");
    DEBUG_PRINT(2, debounceTime[button]);
    DEBUG_PRINT(2, "\n");
  }
  lastState[button] = currentValue;
  
  if (currentValue != buttonState[button]) {
    if ((millis() - debounceTime[button]) > debounce_delay) {
      buttonState[button] = currentValue;
      DEBUG_PRINT(2, "Button ");
      DEBUG_PRINT(2, button);
      DEBUG_PRINT(2, ": value=");
      DEBUG_PRINT(2, buttonState[button]);
      DEBUG_PRINT(2, "\n");
    }
  }
  
  return buttonState[button];
}


boolean checkButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (debounced_read(i, 50) == HIGH)
      return true;
  }
  return false;
}
