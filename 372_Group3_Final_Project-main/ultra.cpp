#include "ultra.h"
#include <Arduino.h>


 void initUltra() {

     DDRH |= (1 << DDH6); //Pin 9 is used for the trigger, this is an output
     DDRB &= ~(1 << DDB4); //Pin 10 is used for the echo, this is an input

 }
