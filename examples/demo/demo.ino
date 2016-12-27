//We always have to include the library
#include <Wire.h>
#include "max7219.h"


/*
 Now we need a max7219 to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 7 is connected to the DataIn  (DIN)
 pin 6 is connected to the CLK     (CLK)
 pin 5 is connected to LOAD        (CS)
 We have only a single MAX72XX.
 */
#define NBR_MTX 8 //Number of displays
MAX7219 max7219=MAX7219(7,6,5, NBR_MTX);


void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  Serial.begin (9600);

  for (int i=0; i< NBR_MTX; i++){
    max7219.shutdown(i,false);
  /* Set the brightness to a high value */
    max7219.setIntensity(i,15);
  /* and clear the display */
    max7219.clearDisplay(i);
  }
  delay(100);
  max7219.clearAll();
 }


void loop(){
  max7219.setChar(0,7);
  max7219.setIntensity(1,3); //Dim the 2nd matrix
  max7219.setCharString(1,'A');
  max7219.setDigit(2,0,10,false);       //a
  max7219.setDigitRaw(2,1,0b00000101);  //r
  max7219.setDigit(2,2,13,false);       //d
  max7219.setDigitRaw(2,3,0b00011100);  //u
  max7219.setDigitRaw(2,4,0b00010000);  //i
  max7219.setDigitRaw(2,5,0b00010101);  //n
  max7219.setDigitRaw(2,6,0b00011101);  //o
  max7219.setDigit(2,7,4,true);
  delay(99999);
  }
