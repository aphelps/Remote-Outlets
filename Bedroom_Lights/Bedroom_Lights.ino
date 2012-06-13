#define clockpin 13 // CI
#define enablepin 10 // EI
#define latchpin 9 // LI
#define datapin 11 // DI
 
// Analog Values
#define PIN_PHOTORESISTOR 0
#define PIN_DIAL          1

#define NumLEDs 1

#define RED   0
#define GREEN 1
#define BLUE  2

int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;
 
void setup() {  
   Serial.begin(9600);
   randomSeed(analogRead(0));

   /* Setup the ShiftBrite pins */ 
   pinMode(datapin, OUTPUT);
   pinMode(latchpin, OUTPUT);
   pinMode(enablepin, OUTPUT);
   pinMode(clockpin, OUTPUT);
   SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
   digitalWrite(latchpin, LOW);
   digitalWrite(enablepin, LOW);
 
 
}
 
void SB_SendPacket() {
 
    if (SB_CommandMode == B01) {
     SB_RedCommand = 120;
     SB_GreenCommand = 100;
     SB_BlueCommand = 100;
    }
 
    SPDR = SB_CommandMode << 6 | SB_BlueCommand >> 4;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_BlueCommand << 4 | SB_RedCommand >> 6;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_RedCommand << 2  | SB_GreenCommand>>8;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_GreenCommand;
    while(!(SPSR & (1<<SPIF)));
 
}
 
void WriteLEDArray() {
 
    SB_CommandMode = B00; // Write to PWM control registers
    for (int h = 0; h<NumLEDs; h++) {
	  SB_RedCommand   = LEDChannels[h][0];
	  SB_GreenCommand = LEDChannels[h][1];
	  SB_BlueCommand  = LEDChannels[h][2];
	  SB_SendPacket();
    }
 
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
 
    SB_CommandMode = B01; // Write to current control registers
    for (int z = 0; z < NumLEDs; z++) SB_SendPacket();
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
 
}
 
#define MODES 7



#define VALUE_MAX 1023
#define VALUE_INC 10

#define DELAY 5

/* Threshold value for the photoresistor */
#define THRESHOLD 650

void LED_set_mode1(int cycle, int value) {
  switch (cycle % MODES) {
    case 0: { // All on
      LEDChannels[0][0] = value;
      LEDChannels[0][1] = value;
      LEDChannels[0][2] = value;      
      break;
    }
    case 1: {  // RED
      LEDChannels[0][RED] = value;
      break;
    }
    case 2: { // Green
      LEDChannels[0][GREEN] = value;
      break;     
    }
    case 3: {  // Blue
      LEDChannels[0][BLUE] = value;
      break;
    }
    case 4: { // Red and Green
      LEDChannels[0][RED] = value;
      LEDChannels[0][GREEN] = value;
      break;
    }
    case 5: {
      LEDChannels[0][GREEN] = value;
      LEDChannels[0][BLUE] = value;      
      break;
    }
    case 6: {
      LEDChannels[0][RED] = value;
      LEDChannels[0][BLUE] = value;      
      break;
    }
  }  
}

void LED_set_random(int cycle, int value) {
  static int last_cycle = -1;
  static double color[3];

  if (cycle != last_cycle) {
    double max = 0;
    for (int i = 0; i < 3; i++) {
      color[i] = random(0, VALUE_MAX);
      if (color[i] > max) max = color[i];
    }
    
    for (int i = 0; i < 3; i++) {
      color[i] = color[i] / max; 
    }
    
    last_cycle = cycle;
  }
  
  for (int i = 0; i < 3; i++) {
    LEDChannels[0][i] = value * color[i];
  }      
}

void loop() {
  static int cycle;
  static int value;
  static int direction = 1;

  LEDChannels[0][0] = 0;
  LEDChannels[0][1] = 0;
  LEDChannels[0][2] = 0;
  
//  int lightvalue = THRESHOLD + 1;
  int lightvalue = analogRead(PIN_PHOTORESISTOR);
  
#ifdef DIAL
  int dialvalue = analogRead(PIN_DIAL);
  mode = map(dialvalue, 250, 870, 0, MODES);
  if (mode == MODES) {
    mode = cycle % MODES;
  }
#endif   

  Serial.print("light ");
  Serial.print(lightvalue);
#ifdef DIAL
  Serial.print("dial ");
  Serial.print(dialvalue);
#endif
  Serial.print(" value ");
  Serial.print(value);
  Serial.print(" cycle ");
  Serial.print(cycle);
  Serial.print("\n"); 
  
  if (lightvalue > THRESHOLD) {    
    if (direction)  {
      value = (value + VALUE_INC);
      if (value > VALUE_MAX) {
        value = VALUE_MAX;  
        cycle++;
        direction = 0;
      }
    } else {
      value = (value - VALUE_INC);
      if (value <= 0) {
        value = 0;
        cycle++;
        direction = 1;       
      }
    }
    
    //  LED_set_mode1(cycle, value);
    LED_set_random(cycle, value);
  } else {
    LEDChannels[0][0] = 0;
    LEDChannels[0][1] = 0;
    LEDChannels[0][2] = 0;
  }
  
  // Send values to LEDs
  WriteLEDArray();
  
  delay(DELAY);
}

