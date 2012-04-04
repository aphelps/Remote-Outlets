// Graphing sketch
 
 
// This program takes ASCII-encoded strings
// from the serial port at 9600 baud and graphs them. It expects values in the
// range 0 to 1023, followed by a newline, or newline and carriage return
 
// Created 20 Apr 2005
// Updated 18 Jan 2008
// by Tom Igoe
// This example code is in the public domain.
 
import processing.serial.*;
 
Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph
 
void setup () {
  // set the window size:
  size(400, 300);        
 
  // List all the available serial ports
  println(Serial.list());
  // I know that the first port in the serial list on my mac
  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  // set inital background:
  background(0);
}
void draw () {
  // everything happens in the serialEvent()
}
 
void serialEvent (Serial myPort) {
  // get the ASCII string: <pin>:<value>,<pin>:<value>, ...\n


  //String line1 = myPort.readStringUntil('\n');
  //if (line1 != null) println("line:" + line1);

  int pin1 = -1, pin2 = -1, pin3 = -1, val1 = -1, val2 = -1, val3 = -1;

  String str1 = myPort.readStringUntil(' ');
  if (str1 != null) {
//    println("str1 = " + str1);
    String[] pin_val = split(str1, ":");
    pin1 = int(trim(pin_val[0]));
    val1 = int(trim(pin_val[1]));
  }

  String str2 = myPort.readStringUntil(' ');
  if (str2 != null) {
//    println("str2 = " + str2);
    String[] pin_val = split(str2, ":");
    pin2 = int(trim(pin_val[0]));
    val2 = int(trim(pin_val[1]));
  }

  String str3 = myPort.readStringUntil('\n');
  if (str3 != null) {
//    println("str3 = " + str3);
    String[] pin_val = split(str3, ":");
    pin3 = int(trim(pin_val[0]));
    val3 = int(trim(pin_val[1]));
  }
  
//  println(str1 + "=" + val1 + " " + str2 + "=" + val2 + " " + str3 + "=" + val3);

  if (val1 != 0) {

    fill(0);
    rect(-1, -1, width + 2, height + 2);

    // trim off any whitespace:
    // convert to an int and map to the screen height:
    float inByte = inByte = map(val1, 0, 1023, 0, height);
 
    // draw the line:
    int radius = 15;
    if (val3 < 5) {
      fill (255, 0, 0);
      radius = 20;
    }
    stroke(127, 34, 255);
    //line(xPos, height, xPos, height - inByte);

    float x = map(val1, 0, 1023, 0, width);
    float y = height - map(val2, 0, 1023, 0, height);
    ellipse(x, y, radius, radius);
 
    // at the edge of the screen, go back to the beginning:
    if (xPos >= width) {
      xPos = 0;
      background(0);
    }
    else {
      // increment the horizontal position:
      xPos++;
    }
  }
}
