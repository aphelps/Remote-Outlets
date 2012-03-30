#ifndef LCD_H
#define LCD_H

#include "Arduino.h"

void LCD_setup();
void LCD_set(int row, int col, String text);
void LCD_loop();

#endif
