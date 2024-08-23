
#include "lcd.h"
#include "timer.h"
#include <Arduino.h>
#include <util/delay.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){ //Initializing the pins on the LCD as outputs

  DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3);
  DDRB |= (1 << DDB4) | (1 << DDB6); //enable and then RS pin respectively 
}



/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){

// Assert bits to pins connected to DB[7:4] on the LCD screen 
//RS low for sending command 

  PORTA = ((PORTA & 0xF0) | (data & 0x0F));

  //RS should be low 
  PORTB &= ~ (1 << PORTB6);
  //Assert high on enable pin
  PORTB |= (1 << PORTB4);
  //Delay 1 microsecond
  delayUs(1);
  //Low on enable pin 
  PORTB &= ~(1 << PORTB4);
  //delay for the command
  delayUs(delay);

}



/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
 //RS low for sending command 
 PORTA = (PORTA & 0XF0) | ((command & 0xF0) >> 4); //Bit mask upper 4 bits of command onto the lower 4 bits of PORTA
 PORTB &= ~ (1 << PORTB6); //RS pin low 
 PORTB |= (1 << PORTB4); //Enable pin high
 delayUs(1); //delay 1 microsecond
 PORTB &= ~ (1 << PORTB4); //Enable pin low
 PORTA = (PORTA & 0xF0) | (command & 0x0F); //Bit mask lower 4 bits of command onto PORTA
 PORTB |= (1 << PORTB4); //Enable pin high
 delayUs(1); //Delay for 1 microsecond
 PORTB &= ~ (1 << PORTB4); //Enable pin low
 delayUs(delay); //delay for the command 

}


/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
  //RS pin high for displaying 
 
  PORTA = (PORTA & 0XF0) | ((character & 0xF0) >> 4); //Bit masking upper 4 bits of character onto the lower 4 bits of PORTA
  PORTB |= (1 << PORTB6); //RS pin goes high
  PORTB |= (1 << PORTB4); //Enable pin goes high
  delayUs(1); //Delay for 1 microsecond
  PORTB &= ~ (1 << PORTB4); //Enable pin goes low
  PORTA = (PORTA & 0xF0) | (character & 0x0F); //Bit masking the lower four bits
  PORTB |= (1 << PORTB4); //Enable pin goes high
  delayUs(1); //Delay for 1 microsecond
  PORTB &= ~ (1 << PORTB4); //Enable pin low
  delayUs(46); //delay for 46 microseconds for a character to write


}


/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){

  while (*string != '\0') { //while loop until it reaches the end of the word 
    writeCharacter(*string); //write character
    string++; //incrementing the address of the pointer 
  }
}

/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){

  unsigned char ddAddress = ((0X80) | (x << 6)) | y; //DB7 always 1, x represents rows, y represents columns 
  eightBitCommandWithDelay(ddAddress, 46); //run eight bit command with the address
	
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  // Delay 50 milliseconds

  delayMs(50);

  // Write 0b0011 to DB[7:4] and delay 4100 microseconds

  // PORTA |= (1 << PORTA0) | (1 << PORTA1); //Setting two least sign bits to 1
  // PORTA &= ~((1 << PORTA2) | (1 << PORTA3)); //Setting upper two to 0

  // delayUs(4100);

  fourBitCommandWithDelay(0b0011, 4100); 
  
  // Write 0b0011 to DB[7:4] and delay 100 microseconds

  fourBitCommandWithDelay(0b0011, 100);

  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay

  fourBitCommandWithDelay(0b0011, 100);

  // write 0b0010 to DB[7:4] and 100us delay.

  fourBitCommandWithDelay(0b0010, 100);

  // Function set in the command table with 53us delay

  eightBitCommandWithDelay(0b00101000, 53);

  // Display off in the command table with 53us delay

  eightBitCommandWithDelay(0b00001000, 53);

  // Clear display in the command table. Remember the delay is longer!!!

  eightBitCommandWithDelay(0b00000001, 3000);

  // Entry Mode Set in the command table.

  eightBitCommandWithDelay(0b0000110, 53);

  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)

  eightBitCommandWithDelay(0b00001100, 53);

  

}



/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
  
}
