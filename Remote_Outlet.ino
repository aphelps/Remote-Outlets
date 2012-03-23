#define NUM_BUTTONS 3
int BUTTONS[NUM_BUTTONS] = {2, 3, 4};

int LED = 10;
int debugLED = 12;
int analogValue = 0;
int remoteIndicator = false;

int lastRemoteIndicator = false;
unsigned long lastSent = 0;

#define DEBUG
#ifdef DEBUG
  #define DEBUG_VERBOSE 1
  #define DEBUG_PRINT(v, x) if (v <= DEBUG_VERBOSE) Serial.print(x)
  #define DEBUG_WRITE(v, x) if (v <= DEBUG_VERBOSE)  Serial.print(x, HEX);
#else
  #define DEBUG_PRINT(v, x)
  #define DEBUG_WRITE(v, x)
#endif

void setup() {
  for (int i; i < NUM_BUTTONS; i++) {
    pinMode(BUTTONS[i], INPUT);  
  }

  pinMode(LED, OUTPUT);
  pinMode(debugLED, OUTPUT); 
  Serial.begin(9600);
}



void loop() {
  if (Serial.available() >= 23) {
    if (Serial.read() == 0x7E) {
#ifdef DEBUG      
      digitalWrite(debugLED, HIGH);
      delay(10);
      digitalWrite(debugLED, LOW);
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
    }
  }
#if 0
  else {
      digitalWrite(debugLED, HIGH);
      delay(1000);
      digitalWrite(debugLED, LOW);
      
      DEBUG_PRINT(2, "No value: ");
      DEBUG_PRINT(2, Serial.available());
      DEBUG_PRINT(2, "\n");
  }
#endif

  
  if (checkButtons()) {
    remoteIndicator = true;
  } else {
    remoteIndicator = false;
  }
  
  if (remoteIndicator != lastRemoteIndicator) {
    DEBUG_PRINT(1, "Changes state to: ");
    DEBUG_PRINT(1, remoteIndicator);
    DEBUG_PRINT(1, "\n");
    if (remoteIndicator == false) setRemoteState(0x4);
    if (remoteIndicator == true)  setRemoteState(0x5);
    lastRemoteIndicator = remoteIndicator;
  }
}

uint8_t command[] = { 0x7E, 0x00, 0x10, 0x17, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
                   0xFF, 0xFE, 
                   0x02,  
                   'D',  '1', 0X00, 
                   0X00
                 };

void setRemoteState(int value) {
  // Command Value
  command[18] = value;
  
  // Checksum is all bytes after length bytes
  long sum = 0x17 + 0xFF + 0xFF + 0xFF + 0xFE + 0x02 + 'D' + '1' + value;
  command[19] = 0xFF - (sum & 0xFF);
  
  Serial.write(command, 20);
  
  DEBUG_PRINT(1, "\nremote state\n");

  delay(1); // Safety pause to avoid overloading serial port if not implemented properly
}
