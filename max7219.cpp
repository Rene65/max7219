/*
 *    max7219.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *
 *
 * History:
 * 2016-9-19   René Wennekes <rene.wennekes AT gmail.com>
 *             Simplyfying library, renaming to max7219 & improve memory usage.
*/
 

#include "max7219.h"

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

MAX7219::MAX7219(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices) {
    SPI_MOSI=dataPin;
    SPI_CLK=clkPin;
    SPI_CS=csPin;
    if(numDevices<=0 || numDevices>8 )
	numDevices=8;
    maxDevices=numDevices;
    pinMode(SPI_MOSI,OUTPUT);
    pinMode(SPI_CLK,OUTPUT);
    pinMode(SPI_CS,OUTPUT);
    digitalWrite(SPI_CS,HIGH);
    SPI_MOSI=dataPin;
    for(uint8_t i=0;i<64;i++) 
	status[i]=0x00;
    for(uint8_t i=0;i<maxDevices;i++) {
	spiTransfer(i,OP_DISPLAYTEST,0);
	//scanlimit is set to max on startup
	setScanLimit(i,7);
	//decode is done in source
	spiTransfer(i,OP_DECODEMODE,0);
	clearDisplay(i);
	//we go into shutdown-mode on startup
	shutdown(i,true);
    }
  }

void MAX7219::spiTransfer(uint8_t device, volatile byte opcode, volatile byte data) {
    //Create an array with the data to shift out
    for(uint8_t i=0;i<maxDevices*2;i++)
	spidata[i]=(byte)0;
    //put our device data into the array
    spidata[device*2+1]=opcode;
    spidata[device*2]=data;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data 
    for(uint8_t i=maxDevices*2;i>0;i--)
 	shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
  }    

uint8_t MAX7219::getDeviceCount() {
    return maxDevices;
  }

void MAX7219::shutdown(uint8_t device, bool b) {
    if(device<0 || device>=maxDevices)
	return;
    if(b)
	spiTransfer(device, OP_SHUTDOWN,0);
    else
	spiTransfer(device, OP_SHUTDOWN,1);
  }
	
void MAX7219::setScanLimit(uint8_t device, uint8_t limit) {
    if(device<0 || device>=maxDevices)
	return;
    if(limit>=0 || limit<8)
    	spiTransfer(device, OP_SCANLIMIT,limit);
  }

void MAX7219::setIntensity(uint8_t device, uint8_t intensity) {
    if(device<0 || device>=maxDevices)
	return;
    if(intensity>=0 || intensity<16)	
	spiTransfer(device, OP_INTENSITY,intensity);
  }

void MAX7219::clearDisplay(uint8_t device) {
    if(device<0 || device>=maxDevices)
	return;
    for(int i=0;i<8;i++) {
	status[device*8+i]=0;
	spiTransfer(device, i+1,status[device*8+i]);
    }
  }

void MAX7219::clearAll() {
   for (uint8_t i=0;i<maxDevices;i++) clearDisplay(i);
  }

void MAX7219::setLed(uint8_t device, uint8_t row, uint8_t column, boolean state) {
    if(device<0 || device>=maxDevices)
	return;
    if(row<0 || row>7 || column<0 || column>7)
	return;
    byte val=0x00;
    val=B10000000 >> column;
    if(state)
	status[device*8+row]=status[device*8+row]|val;
    else {
	val=~val;
	status[device*8+row]=status[device*8+row]&val;
    }
    spiTransfer(device, row+1,status[device*8+row]);
  }
	
void MAX7219::setRow(uint8_t device, uint8_t row, uint8_t value) {
    if(device<0 || device>=maxDevices)
	return;
    if(row<0 || row>7)
	return;
    status[device*8+row]=value;
    spiTransfer(device, row+1,status[device*8+row]);
  }
    
void MAX7219::setColumn(uint8_t device, uint8_t col, uint8_t value) {
    if(device<0 || device>=maxDevices)
	return;
    if(col<0 || col>7) 
	return;
    uint8_t val;
    for(int row=0;row<8;row++) {
	val=value >> (7-row);
	val=val & 0x01;
	setLed(device,row,col,val);
    }
  }

void MAX7219::setDigit(uint8_t device, uint8_t digit, uint8_t value, boolean dp) {
    if(device<0 || device>=maxDevices)
	return;
    if(digit<0 || digit>7 || value>16)
	return;
    uint8_t v=seg7Chartable[value];
    if(dp)
	v|=B10000000;
    status[device*8+digit]=v;
    spiTransfer(device, digit+1,v);
  }

void MAX7219::setDigitRaw(uint8_t device, uint8_t digit, uint8_t value) {
    if(device<0 || device>=maxDevices)
	return;
    if(digit<0 || digit>7)
	return;
    uint8_t v=value;
    status[device*8+digit]=v;
    spiTransfer(device, digit+1,v);
  }

void MAX7219::setChar(uint8_t device, uint8_t value) {
  for (uint8_t i=0; i<8;i++) {
      /* for the new chinese displays the number will be display normally when the data-in at the right side of
         the display, so the chain of displays wil be from left to right 7-6-5-4-3-2-1-0.
         This means that the sequense of device numbers is diffrent as the 7-segment digits.
         The old displays the data-in and out pin is other way arroud on relation of the numberdisplay, however
         fysical layout (because the max7219 chip) requires a 90 degrees counterclockwise display (slow).*/
      setRow(device,i, matrix8x8Chartable[value][i]); //For new chinese displays
      //setRow(device,i, matrix8x8Chartable[value][7-i]); //For new chinese displays - still working on it - need bitmirorring
      //setRow(maxDevices-1-device,i, matrix8x8Chartable[value][i]); //change device horizontaly
      //setColumn(device,i, matrix8x8Chartable[value][7-i]); //For old chines displays (slow)
      } 
  }

void MAX7219::setCharString(uint8_t device, char value) {
     uint8_t charnum=36;
     if (value ==' ') charnum=36;
     if (value =='+') charnum=36;
     if (value ==':') charnum=37;
     if (value =='-') charnum=38;
     if (value =='.') charnum=39;
     if (value =='#') charnum=40;  //replace by dot middle
     if ((value >='0')&&(value <='9')) charnum= (value-'0');
     if ((value >='A')&&(value <='Z')) charnum= (value-'A' + 10);
     if ((value >='a')&&(value <='z')) charnum= (value-'a' + 10);     
     setChar(device, charnum);
  }  
