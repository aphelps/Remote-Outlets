#include "Buttons.h"
#include "Debug.h"

void setup()
{
  Serial.begin(9600);          //  setup serial
}

void loop()
{
  int val5, val6, val7;
  
  val5 = analogRead(5);    // read the input pin
  val6 = analogRead(6);    // read the input pin
  val7 = analogRead(7);    // read the input pin

  Serial.print("5:");
  Serial.print(val5);
  Serial.print(" 6:");
  Serial.print(val6);
  Serial.print(" 7:");
  Serial.print(val7); 
  Serial.print("\n");

  delay(10);
}
