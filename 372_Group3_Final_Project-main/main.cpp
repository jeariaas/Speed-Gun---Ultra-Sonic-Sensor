#include <Arduino.h>

// 

//----------------------------------------------------------------------//




#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "switch.h"
#include "timer.h"
#include "lcd.h"
#include "ledMatrix.h"
#include "ultra.h"

// definitions

volatile int timerOverFlowCount = 0;


enum states_button { //enumerations for the different states of the switch 
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
};

volatile states_button currentState = wait_press; //Initializing the current state to wait_press


int main(){


  initTimer1();
  initTimer0();

  sei(); // Enable global interrupts.
  initLCD();
  initSwitchPB3();

  int speedFromUltra = 0;

  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("Speed: ");
  moveCursor(1, 0);  // moves the cursor to 1,0 position
  writeString("0 mph ");

   

// while loop

while (1) { //infinite loop 


  /* 
  
  //need to obtain speed from ultra sensor
  //speedFromUltra = 
  //display speed:
  //moveCursor(0, 0); // moves the cursor to 0,0 position
  //writeString("Speed: ");
  //moveCursor(1, 0);  // moves the cursor to 1,0 position
  //writeString("%d", speedFromUltra); //idk if this is the correct syntax
  
  if () { // if speed is above 15 mph

    displayStopSign();

  }


  */

     switch(currentState){ //state machine to check the what state the push button is pressed
      case wait_press:
          break;
      case debounce_press:
          delayMs(1); //delay for 1 ms to make sure the noise has gone away 
          currentState = wait_release;
      case wait_release:
          break;
      case debounce_release:
          delayMs(1); //delay for 1 ms to make sure the noise has gone away 
          turnOffDisplay();
          currentState = wait_press;
          break;
    }
}
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/

//Include timer 0 interrupt ISR

ISR(TIMER0_COMPA_vect) { //ISR for timer0
  //Serial.println("hi");
  timerOverFlowCount++; //Keeping track of the overflow count for timer0
}


ISR(PCINT0_vect){ //ISR for switch interrupt 
  
  if (currentState == wait_press) { //if the current state is in the wait press
    currentState = debounce_press; //switch to debounce state where it will wait for the noise to disappear 
  }

  else if (currentState == wait_release) { //if the current state is in the wait release state 
    
    currentState = debounce_release; //switch to the debounce state where it will wait for the noise to disappear
  }

}
