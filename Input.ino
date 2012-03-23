int buttonState[NUM_BUTTONS];
int lastState[NUM_BUTTONS];
long debounceTime[NUM_BUTTONS];

void debounce_init() {
  for (int i = 0; i < NUM_BUTTONS) {
    buttonState[i] = LOW;
    lastState[i] = LOW;
    debounceTime = 0;
  }
}
  
  
int debounced_read(int button, int debounce_delay) {
  int currentValue = digitalRead(BUTTONS[button]);
  
  if (currentValue != lastState[button]) {
    debounceTime = millis();  
  }
  lastState[button] = currentValue;
  
  if ((millis() - debounceTime[button]) > debounce_delay) {
    buttonState[button] = currentValue;
  }
  
  return buttonState[button];
}


boolean checkButtons() {
  for (int i; i < NUM_BUTTONS; i++) {
//    if (digitalRead(BUTTONS[i]) == HIGH) 
    if (debounced_read(i, 50) == HIGH)
      return true;
  }
  return false;
}
