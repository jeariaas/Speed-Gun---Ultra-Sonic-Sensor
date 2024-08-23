#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3, header pin 50, and sets it into input mode
 */
void initSwitchPB3(){
    
    DDRB &= ~(1<<DDB3); //Sets swicth button as an input

    PORTB |= (1<<DDB3); //Pull up resistor for stable input 

    PCICR |= (1<<PCIE0); //Initializing the group of pins
    PCMSK0 |= (1<<PCINT3); //Initializing the local pin
  
}
