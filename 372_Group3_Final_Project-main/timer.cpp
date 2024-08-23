// Author: 


#include "timer.h"
#include <Arduino.h>
//You many use any timer you wish for the microsecond delay and the millisecond delay

extern volatile int timerOverFlowCount;

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){

    TCCR1A &= ~ (1 << WGM10); //Initializing to CTC mode
    TCCR1A &= ~ (1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~ (1 << WGM13);

	
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){ 

    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Setting the prescaler 8
    TCCR1B |= (1 << CS11);
    OCR1AH = 0;
    OCR1AL = 2;

    
    unsigned int timerOverFlowCount1 = 0; //counter

    TIFR1 |= (1<<OCF1A);
    TCNT1 = 0;

    while (timerOverFlowCount1 < delay){ //while the counter is less than the delay 
        if((TIFR1 & (1<<OCF1A))) { //every time counter raises a flag
            timerOverFlowCount1++; //increment counter
            TIFR1 |= (1 << OCF1A); //set timer to start counting again 
        }
    }

}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){

    TCCR0A &= ~ (1 << WGM00); //Initializing the mode to CTC 
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    TIMSK0 |= (1 << OCIE0A);

}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
   
    TCNT0 = 0; //comparing counter to the compare value (249) which will happen every millisecond 
    OCR0A = 249;
    TCCR0B |= (1 << CS00) | (1 << CS01); //Setting the prescaler to 64

    while (timerOverFlowCount <= delay); // while loop that is counting up to specified delay 

    timerOverFlowCount = 0;

}
