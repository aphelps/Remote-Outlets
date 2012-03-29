#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 6, 7, 8, 9, 10);



void LCD_setup() 
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
}

void LCD_set(int row, int col, String text) 
{
  lcd.setCursor(col, row);
  lcd.print(text);
}
