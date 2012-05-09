#define clockpin 13 // CI
#define enablepin 10 // EI
#define latchpin 9 // LI
#define datapin 11 // DI

// Analog Values
#define PIN_PHOTORESISTOR 0
#define PIN_DIAL          1

#define NumLEDs 1
 
int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;
 
void setup() {
  
   Serial.begin(9600);
   
//   pinMode(photoresistor, INPUT);
 
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
    for (int h = 0;h<NumLEDs;h++) {
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
 
int cycle = 0;
#define MODES 7

int value = 0;
int direction = 0;

#define VALUE_MAX 1024
#define VALUE_INC 1

#define DELAY 10

void loop() {
#if 1
  LEDChannels[0][0] = 0;
  LEDChannels[0][1] = 0;
  LEDChannels[0][2] = 0;
  
  int lightvalue = analogRead(PIN_PHOTORESISTOR);

  
  int mode = cycle % MODES;
  
#if 0
  int dialvalue = analogRead(PIN_DIAL);
  mode = map(dialvalue, 250, 870, 0, MODES);
  if (mode == MODES) {
    mode = cycle % MODES;
  }
#endif

  switch (mode) {
    case 0: {
      LEDChannels[0][0] = value;
      LEDChannels[0][1] = value;
      LEDChannels[0][2] = value;      
      break;
    }
    case 1: {
      LEDChannels[0][0] = value;
      break;
    }
    case 2: {
      LEDChannels[0][1] = value; 
      break;     
    }
    case 3: {
      LEDChannels[0][2] = value;      
      break;
    }
    case 4: {
      LEDChannels[0][0] = value;
      LEDChannels[0][1] = value;
      break;
    }
    case 5: {
      LEDChannels[0][1] = value;
      LEDChannels[0][2] = value;      
      break;
    }
    case 6: {
      LEDChannels[0][0] = value;
      LEDChannels[0][2] = value;      
      break;
    }

  }
  #else
  LEDChannels[0][0] = value;
  LEDChannels[0][1] = value;
  LEDChannels[0][2] = value;
  #endif


  WriteLEDArray();
  Serial.print("light ");
  Serial.print(lightvalue);
  Serial.print("dial ");
  Serial.print(dialvalue);
  Serial.print(" value ");
  Serial.print(value);
  Serial.print(" cycle ");
  Serial.print(cycle);
  Serial.print(" mode ");
  Serial.print(mode);
  Serial.print("\n"); 
  
  if (lightvalue > 400) {  
    if (value + VALUE_INC >= VALUE_MAX)
      cycle++;
  
    value = (value + VALUE_INC) % VALUE_MAX;
  } else {
    value = 0;  
  }
  
  delay(DELAY);
}

