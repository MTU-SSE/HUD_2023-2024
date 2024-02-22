#include <CAN.h>


/*
*voodoo magic
*/
int toUnsigned16FromCAN(){
  int mostSignifigant = CAN.read();
  int leastSignifigant =  CAN.read();

  mostSignifigant&=0xff;
  leastSignifigant&=0xff;

  mostSignifigant <<= 8;
  leastSignifigant <<= 0;

  return mostSignifigant | leastSignifigant;
}