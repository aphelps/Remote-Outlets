void sensor_light_led(int pin, int value, void *arg)
{
  int led_pin = (int)arg;
#ifdef DEBUG
  if ((led_pin < 0) || (led_pin > 14))
    return;
#endif

  DEBUG_PRINT(2, "sensor_light_led\n");
  digitalWrite(led_pin, value);
}

void sensor_set_xbee(int pin, int value, void *arg)
{
  char led_pin = (int)arg & 0xFF;
#ifdef DEBUG
  if ((led_pin < '0') || (led_pin > '7'))
    return;
#endif

  DEBUG_PRINT(2, "sensor_set_xbee\n");

  if (value == HIGH) {
    setRemoteState(led_pin, 0x5);
  } else {
    setRemoteState(led_pin, 0x4);
  } 
}

/* Scroll to the next menu item */
void sensor_menu_next(int pin, int value, void *arg)
{
  DEBUG_PRINT(3, "sensor_menu_next\n");
  if (value == HIGH) {
    DEBUG_PRINT(1, "sensor_menu_next HIGH\n");
    Menu *menu = (Menu *)arg;
    if (menu->selected) {
      menu->enter();
    } else {
      menu->next();
    }
  }
}

/* Scroll to the previous menu item */
void sensor_menu_prev(int pin, int value, void *arg)
{
  DEBUG_PRINT(3, "sensor_menu_prev\n");
  if (value == HIGH) {
    DEBUG_PRINT(1, "sensor_menu_prev HIGH\n");
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
void sensor_menu_select(int pin, int value, void *arg)
{
  DEBUG_PRINT(3, "sensor_menu_select\n");
  if (value == HIGH) {
    DEBUG_PRINT(1, "sensor_menu_select HIGH\n");
    Menu *menu = (Menu *)arg;
    if (menu->selected)
      menu->action();
    else
      menu->enter();
  }
}

#define JOYSTICK_DELAY_MS   500

#define JOYSTICK_VERT_DOWN  300
#define JOYSTICK_VERT_UP    500
void joystick_menu_vert(int pin, int value, void *arg)
{
  Menu *menu = (Menu *)arg;

  DEBUG_PRINT(3, "joystick_menu_vert ");
  DEBUG_PRINT(3, value);
  DEBUG_PRINT(3, "\n");

  static unsigned int last_action_ms = 0;
  if (last_action_ms &&
      (last_action_ms + JOYSTICK_DELAY_MS > millis()))
  {
    return;
  }

  if (value < JOYSTICK_VERT_DOWN) {
    DEBUG_PRINT(1, "joystick_menu_vert down\n");
    if (menu->selected) {
      
    } else {
      /* Next menu item */
      menu->next();
      last_action_ms = millis();
    }
    
  } else if (value > JOYSTICK_VERT_UP) {
    DEBUG_PRINT(1, "joystick_menu_vert up\n");
    if (menu->selected) {
      
    } else {
      /* Previous menu item */
      menu->prev();
      last_action_ms = millis();
    }
  }
}

#define JOYSTICK_HORZ_RIGHT   300
#define JOYSTICK_HORZ_LEFT    500
void joystick_menu_horz(int pin, int value, void *arg)
{
  Menu *menu = (Menu *)arg;

  DEBUG_PRINT(3, "joystick_menu_horz ");
  DEBUG_PRINT(3, value);
  DEBUG_PRINT(3, "\n");

  static unsigned int last_action_ms = 0;
  if (last_action_ms &&
      (last_action_ms + JOYSTICK_DELAY_MS > millis()))
  {
    return;
  }

  if (value < JOYSTICK_HORZ_RIGHT) {
    DEBUG_PRINT(1, "joystick_menu_horz right\n");
    if (menu->selected) {

    } else {

    }
  } else if (value > JOYSTICK_HORZ_LEFT) {
    DEBUG_PRINT(1, "joystick_menu_horz left\n");
    if (menu->selected) {
      /* Exit the sub-menu */
      menu->enter();
      last_action_ms = millis();
    } else {
    }
  }

}

#define JOYSTICK_SELECT_DOWN  50
#define JOYSTICK_SELECT_UP    50
void joystick_menu_select(int pin, int value, void *arg)
{
  Menu *menu = (Menu *)arg;

  DEBUG_PRINT(3, "joystick_menu_select ");
  DEBUG_PRINT(3, value);
  DEBUG_PRINT(3, "\n");

  static unsigned int last_action_ms = 0;
  if (last_action_ms &&
      (last_action_ms + JOYSTICK_DELAY_MS > millis()))
  {
    return;
  }

  if (value < JOYSTICK_SELECT_DOWN) {
    DEBUG_PRINT(1, "joystick_menu_select down\n");
    Menu *menu = (Menu *)arg;
    if (menu->selected) {
      menu->action();
      last_action_ms = millis();
    } else {
      menu->enter();
      last_action_ms = millis();
    }
  } else if (value > JOYSTICK_SELECT_UP) {
    
  }
}



/******************************************************************************
 * Menus Actions
 *****************************************************************************/

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
  item->actionText = String("Value: " + String(count));
  menu.display();
  return 0;
}


/*
 * MenuItem action which will toggle the value of a pin on the remote Xbee
 */
int menu_toggle_xbee(MenuItem *item, void *arg)
{
  static int value = HIGH;
  char remote_pin = (int)arg & 0xFF;
#ifdef DEBUG
  if ((remote_pin < '0') || (remote_pin > '7'))
    return 0;
#endif

  DEBUG_PRINT(2, "sensor_set_xbee\n");

  if (value == HIGH) {
    setRemoteState(remote_pin, 0x5);
    value = LOW;
    return HIGH;
  } else {
    setRemoteState(remote_pin, 0x4);
    value = HIGH;
    return LOW;
  }
}
