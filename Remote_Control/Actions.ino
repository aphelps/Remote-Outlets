void button_set_lcd(int button, int value, void *arg)
{
  String text = String(String("button ") + String(button) + String(":") + String(value));
  LCD_set(button - 1, 0, text, true);
}

void button_light_led(int button, int value, void *arg)
{
  int pin = (int)arg;
#ifdef DEBUG
  if ((pin < 0) || (pin > 14))
    return;
#endif

  DEBUG_PRINT(2, "button_light_led\n");
  digitalWrite(pin, value);
}

void button_set_xbee(int button, int value, void *arg)
{
  char pin = (int)arg & 0xFF;
#ifdef DEBUG
  if ((pin < '0') || (pin > '7'))
    return;
#endif

  DEBUG_PRINT(2, "button_set_xbee\n");

  if (value == HIGH) {
    setRemoteState(pin, 0x5);
  } else {
    setRemoteState(pin, 0x4);
  } 
}

/* Scroll to the next menu item */
void button_menu_next(int button, int value, void *arg)
{
  DEBUG_PRINT(1, "button_menu_next");
  if (value == HIGH) {
    Menu *menu = (Menu *)arg;
    if (menu->selected) {
      menu->enter();
    } else {
      menu->next();
    }
  }
}

/* Scroll to the previous menu item */
void button_menu_prev(int button, int value, void *arg)
{
  DEBUG_PRINT(1, "button_menu_prev");
  if (value == HIGH) {
    Menu *menu = (Menu *)arg;
    if (menu->selected) {
      menu->enter();
    } else {
      menu->prev();
    }
  }
}

/*
 * MenuItem action which will select the current item and invoke the action
 * of the currently selected item.
 */
void button_menu_select(int button, int value, void *arg)
{
  DEBUG_PRINT(1, "button_menu_select");
  if (value == HIGH) {
    Menu *menu = (Menu *)arg;
    if (menu->selected)
      menu->action();
    else
      menu->enter();
  }
}

/* MenuItem test action */
int menu_test(MenuItem *item, void *arg) 
{
  DEBUG_PRINT(1, "menu_test\n");
  LCD_set(1, 0, "Action test", true);
  return 1;
}

int menu_count(MenuItem *item, void *arg) 
{
  static int count = 0;
  //Menu *menu = (Menu *)arg;
  count++;
  item->selectedText = String("Value: " + count);
  menu.display();
  return 0;
}


/*
 * MenuItem action which will toggle the value of a pin on the remote Xbee
 */
int menu_toggle_xbee(MenuItem *item, void *arg)
{
  static int value = HIGH;
  char pin = (int)arg & 0xFF;
#ifdef DEBUG
  if ((pin < '0') || (pin > '7'))
    return 0;
#endif

  DEBUG_PRINT(2, "button_set_xbee\n");

  if (value == HIGH) {
    setRemoteState(pin, 0x5);
    value = LOW;
    return HIGH;
  } else {
    setRemoteState(pin, 0x4);
    value = HIGH;
    return LOW;
  }
}
