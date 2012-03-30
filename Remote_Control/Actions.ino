void action_set_lcd(int button, int value) 
{
  String text = String(String("button ") + String(button) + String(":") + String(value));
  LCD_set(button - 1, 0, text);
}

void action_light_led(int button, int value) 
{
  digitalWrite(LED, value);
}
