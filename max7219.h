/*
 *    max7219.h - A library for controling Leds with a MAX7219/MAX7221
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

#ifndef max7219_h
#define max7219_h

#include <Arduino.h>

/*
//  --- DP - G - F -E - D - C - B -A
const static uint8_t seg7Chartable[] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111,  // 9
  0b01110111,  // A
  0b01111100,  // B
  0b01011000,  // C
  0b01011110,  // D
  0b01111001,  // E
  0b01110001,  // F
};     
*/

//  --- DP - A - B -C - D -E - F -G
const static uint8_t seg7Chartable[] = {
    0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,
    0b01111111,0b01111011,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,
};

const static byte matrix8x8Chartable[41][8]={
	{0b00111100,
	 0b01100110,
	 0b01000110,
	 0b01001010,
	 0b01010010,
	 0b01100010,
	 0b01100110,
	 0b00111100}, //0
	
	{0b00011000,
	 0b00111000,
	 0b01111000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b01111110},  //1
	 
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b00000110,
	 0b00011100,
	 0b00110000,
	 0b01100010,
	 0b01111110},//2
	 
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b00001100,
	 0b00001100,
	 0b01000010,
	 0b01100110,
	 0b00111100},//3
	
	{0b00001100,
	 0b00011100,
	 0b00110100,
	 0b01100100,
	 0b01111110,
	 0b00000100,
	 0b00000100,
	 0b00000100},//4
	
	{0b01111110,
	 0b01100010,
	 0b01100000,
	 0b00111100,
	 0b00000110,
	 0b01000010,
	 0b01100110,
	 0b00111100},//5
	
	{0b00111100,
	 0b01100110,
	 0b01100000,
	 0b01111100,
	 0b01100110,
	 0b01000010,
	 0b01100110,
	 0b00111100},//6
	
	{0b01111110,
	 0b00000110,
	 0b00000110,
	 0b00001100,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000},//7
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b00111100,
	 0b00111100,
	 0b01000010,
	 0b01100110,
	 0b00111100},//8
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01100110,
	 0b00111110,
	 0b00000110,
	 0b01100110,
	 0b00111100},//9
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01111110,
	 0b01111110,
	 0b01000010,
	 0b01000010,
	 0b01000010},//A
	
	{0b01111100,
	 0b01000110,
	 0b01000010,
	 0b01111100,
	 0b01111100,
	 0b01000010,
	 0b01000110,
	 0b01111100,},//B
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01000000,
	 0b01000000,
	 0b01000010,
	 0b01100110,
	 0b00111100,},//C
	
	{0b01111100,
	 0b01000110,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000110,
	 0b01111100,},//D
	
	{0b01111110,
	 0b01000000,
	 0b01000000,
	 0b01111000,
	 0b01111000,
	 0b01000000,
	 0b01000000,
	 0b01111110,},//E
	
	{0b01111110,
	 0b01000000,
	 0b01000000,
	 0b01111000,
	 0b01111000,
	 0b01000000,
	 0b01000000,
	 0b01000000,},//F
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01000000,
	 0b01001110,
	 0b01000010,
	 0b01100110,
	 0b00111100,},//G
	
	{0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01111110,
	 0b01111110,
	 0b01000010,
	 0b01000010,
	 0b01000010,},//H
	
	{0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,},//I
	
	{0b00000110,
	 0b00000110,
	 0b00000110,
	 0b00000110,
	 0b00000110,
	 0b01000110,
	 0b01101100,
	 0b00111100,},//J
	
	{0b01000110,
	 0b01001100,
	 0b01011000,
	 0b01110000,
	 0b01110000,
	 0b01011000,
	 0b01001100,
	 0b01000110,},//K
	
	{0b01000000,
	 0b01000000,
	 0b01000000,
	 0b01000000,
	 0b01000000,
	 0b01000000,
	 0b01000000,
	 0b01111110,},//L
	
	{0b01000010,
	 0b01100110,
	 0b01011010,
	 0b01011010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,},//M
	
	{0b01000010,
	 0b01100010,
	 0b01010010,
	 0b01010010,
	 0b01001010,
	 0b01001010,
	 0b01000110,
	 0b01000010,},//N
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01100110,
	 0b00111100,},//O
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01100110,
	 0b01111100,
	 0b01000000,
	 0b01000000,
	 0b01000000,},//P
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01000010,
	 0b01011010,
	 0b01001110,
	 0b01100110,
	 0b00111100,},//Q
	
	{0b00111100,
	 0b01100110,
	 0b01000010,
	 0b01100110,
	 0b01111100,
	 0b01011000,
	 0b01001100,
	 0b01000110,},//R
	
	{0b00111100,
	 0b01100110,
	 0b01000000,
	 0b00111000,
	 0b00011100,
	 0b00000010,
	 0b01100110,
	 0b00111100,},//S
	
	{0b01111110,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,
	 0b00011000,},//T
	
	{0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01100110,
	 0b00111100,},//U
	
	{0b01000010,
	 0b01000010,
	 0b01000010,
	 0b00100100,
	 0b00100100,
	 0b00100100,
	 0b00011000,
	 0b00011000,},//V
	
	{0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01000010,
	 0b01011010,
	 0b01011010,
	 0b01011010,
	 0b00111100,},//W
	
	{0b01000010,
	 0b01000010,
	 0b00100100,
	 0b00011000,
	 0b0001000,
	 0b00100100,
	 0b01000010,
	 0b01000010,},//X
	
	{0b01000010,
	 0b01000010,
	 0b01000010,
	 0b00100110,
	 0b00011110,
	 0b00000010,
	 0b01000110,
	 0b00111100,},//Y
	
	{0b01111110,
	 0b00000010,
	 0b00000100,
	 0b00001000,
	 0b00010000,
	 0b00100000,
	 0b01000000,
	 0b01111110,},//Z
	
	{0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000},// blank space 36
	 
	{0b00000000,
	 0b00011000,
	 0b00011000,
	 0b00000000,
	 0b00000000,
	 0b00011000,
	 0b00011000,
	 0b00000000},// : 37
	 
	{0b00000000,
	 0b00000000,
	 0b00000000,
	 0b01111110,
	 0b01111110,
	 0b00000000,
	 0b00000000,
	 0b00000000},// - 38
	
	{0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00011000,
	 0b00011000},// . 39

	{0b00000000,
	 0b00000000,
	 0b00000000,
	 0b00011000,
	 0b00011000,
	 0b00000000,
	 0b00000000,
	 0b00000000}//Dot middle 40
};


//class LedControl {
class MAX7219 {
 private :
    /* The array for shifting the data to the devices */
    uint8_t spidata[16];
    /* Send out a single command to the device */
    void spiTransfer(uint8_t matrix, uint8_t opcode, uint8_t data);

    /* We keep track of the led-status for all 8 devices in this array */
    byte status[64];
    /* Data is shifted out of this pin*/
    uint8_t SPI_MOSI;
    /* The clock is signaled on this pin */
    uint8_t SPI_CLK;
    /* This one is driven LOW for chip selectzion */
    uint8_t SPI_CS;
    /* The maximum number of devices we use */
    uint8_t maxDevices;
    
 public:
    /* 
     * Create a new controler 
     * Params :
     * dataPin		pin on the Arduino where data gets shifted out
     * clockPin		pin for the clock
     * csPin		pin for selecting the device 
     * numDevices	maximum number of devices that can be controled
     */
    MAX7219(uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices=1);

    /*
     * Gets the number of devices attached to this LedControl.
     * Returns :
     * int	the number of devices on this LedControl
     */
    uint8_t getDeviceCount();

    /* 
     * Set the shutdown (power saving) mode for the device
     * Params :
     * addr	The address of the display to control
     * status	If true the device goes into power-down mode. Set to false
     *		for normal operation.
     */
    void shutdown(uint8_t matrix, bool status);

    /* 
     * Set the number of digits (or rows) to be displayed.
     * See datasheet for sideeffects of the scanlimit on the brightness
     * of the display.
     * Params :
     * addr	address of the display to control
     * limit	number of digits to be displayed (1..8)
     */
    void setScanLimit(uint8_t matrix, uint8_t limit);

    /* 
     * Set the brightness of the display.
     * Params:
     * addr		the address of the display to control
     * intensity	the brightness of the display. (0..15)
     */
    void setIntensity(uint8_t matrix, uint8_t intensity);

    /* 
     * Switch all Leds on the display off. 
     * Params:
     * addr	address of the display to control
     */
    void clearDisplay(uint8_t matrix);
    
    void clearAll();

    /* 
     * Set the status of a single Led.
     * Params :
     * addr	address of the display 
     * row	the row of the Led (0..7)
     * col	the column of the Led (0..7)
     * state	If true the led is switched on, 
     *		if false it is switched off
     */
    void setLed(uint8_t matrix, uint8_t row, uint8_t col, boolean state);

    /* 
     * Set all 8 Led's in a row to a new state
     * Params:
     * addr	address of the display
     * row	row which is to be set (0..7)
     * value	each bit set to 1 will light up the
     *		corresponding Led.
     */
    void setRow(uint8_t matrix, uint8_t row, uint8_t value);

    /* 
     * Set all 8 Led's in a column to a new state
     * Params:
     * addr	address of the display
     * col	column which is to be set (0..7)
     * value	each bit set to 1 will light up the
     *		corresponding Led.
     */
    void setColumn(uint8_t matrix, uint8_t col, uint8_t value);

    /* 
     * Display a hexadecimal digit on a 7-Segment Display
     * Params:
     * addr	address of the display
     * digit	the position of the digit on the display (0..7)
     * value	the value to be displayed. (0x00..0x0F)
     * dp	sets the decimal point.
     */
    void setDigit(uint8_t matrix, uint8_t digit, uint8_t value, boolean dp);

    void setDigitRaw(uint8_t matrix, uint8_t digit, uint8_t value);

    void setChar(uint8_t digit, uint8_t value);

    void setCharString(uint8_t digit, char value);
};

#endif	//max7219.h



