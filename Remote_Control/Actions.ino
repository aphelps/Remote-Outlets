void action_set_lcd(int button, int value, void *arg)
{
  String text = String(String("button ") + String(button) + String(":") + String(value));
  LCD_set(button - 1, 0, text, true);
}

void action_light_led(int button, int value, void *arg)
{
  int pin = (int)arg;
#ifdef DEBUG
  if ((pin < 0) || (pin > 14))
    return;
#endif

  DEBUG_PRINT(2, "action_light_led\n");
  digitalWrite(pin, value);
}

void action_set_xbee(int button, int value, void *arg)
{
  char pin = (int)arg & 0xFF;
#ifdef DEBUG
  if ((pin < '0') || (pin > '7'))
    return;
#endif

  DEBUG_PRINT(2, "action_set_xbee\n");

  if (value == HIGH) {
    setRemoteState(pin, 0x5);
  } else {
    setRemoteState(pin, 0x4);
  } 
}
