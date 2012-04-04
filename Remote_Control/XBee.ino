#include <XBee.h>

uint8_t command[] = { 0x7E, 0x00, 0x10, 0x17, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
                   0xFF, 0xFE, 
                   0x02,  
                   'D',  '1', 0X00, 
                   0X00
                 };

void setRemoteState(int pin, uint8_t value) {
  // Command Value
  command[17] = pin;
  command[18] = value;
  
  // Checksum is all bytes after length bytes
  long sum = 0x17 + 0xFF + 0xFF + 0xFF + 0xFE + 0x02 + 'D' + pin + value;
  command[19] = 0xFF - (sum & 0xFF);
  
  Serial.write(command, 20);
  
  DEBUG_PRINT(1, "\nremote state: pin=");
  DEBUG_PRINT(1, pin);
  DEBUG_PRINT(1, " value=");
  DEBUG_PRINT(1, value);
  DEBUG_PRINT(1, "\n");

  delay(1); // Safety pause to avoid overloading serial port if not implemented properly
}

int analogValue = 0;

/* Read data from the Xbee over the serial port */
void serialEvent() 
{
  if (Serial.available() >= 23) {
    if (Serial.read() == 0x7E) {
#ifdef DEBUG      
      digitalWrite(debugLED, HIGH);
      debugLEDTime = millis() + 500;
#endif

      for (int i = 0; i < 20; i++) {
        byte discard = Serial.read(); 
      } 
      int analogHigh = Serial.read();
      int analogLow = Serial.read();
      analogValue = analogLow + (analogHigh * 256);
      
      DEBUG_PRINT(2, "Read value ");
      DEBUG_PRINT(2, analogValue);
      DEBUG_PRINT(2, "\n");

      String text = String(String("Xbee value: ") + String(analogValue));
      LCD_set(0, 0, text);
    }
  }
}
