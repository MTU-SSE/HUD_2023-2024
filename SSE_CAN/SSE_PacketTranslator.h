#include <CAN.h>

/*
*takes the next CAN 8 bit packet segments and formats it into a fitting data type
*
*@return correctly formatted 8 bit integer retrieved from a packet segment 
*/
uint8_t uint8_tFromCAN(){
  return CAN.read();
}


/*
*takes the next two CAN 8 bit packet segments and formats them 
*into a singular 16 bit unsigned integer
*
*@return correctly formatted 16 bit integer retrieved from a packet segment 
*/
uint16_t uint16_tFromCAN(){
  int mostSignifigant = CAN.read(); //first 8 bits
  int leastSignifigant =  CAN.read(); //last 8 bits

  //make sure no "funny business" is going on by using an 8 bit mask
  mostSignifigant&=0xff;
  leastSignifigant&=0xff;

  //shifts bits to their correct relative location
  mostSignifigant <<= 8;
  leastSignifigant <<= 0;

  //operationally or the two 8 bits together to make a 16 bit integer
  return mostSignifigant | leastSignifigant;
}

/*
*takes the next two CAN 8 bit packet segments and formats them 
*into a singular 16 bit signed integer
*
*@return correctly formatted 16 bit integer retrieved from a packet segment 
*/
int16_t int16_tFromCAN(){
  int mostSignifigant = CAN.read(); //first 8 bits
  int leastSignifigant =  CAN.read(); //last 8 bits

  //make sure no "funny business" is going on by using an 8 bit mask
  mostSignifigant&=0xff;
  leastSignifigant&=0xff;

  //shifts bits to their correct relative location
  mostSignifigant <<= 8;
  leastSignifigant <<= 0;

  //operationally or the two 8 bits together to make a 16 bit integer
  return mostSignifigant | leastSignifigant;
}

/*
*skips a singular 8 bit packet segment
*
*@return returns a 1 on completion
*/
void ignoreCANPacket(){
  CAN.read();

  return 1;
}

/*
*skips multiple 8 bit packet segments based on user input
*
*@param numberToSkip number of packets the user wants to ignore max of 8 ignores
*@return returns a 1 on completion
*/
void ignoreMultipleCANPackets(uint8_t numberToSkip){

  for (uint8_t i = 0; i < numberToSkip; i++){
    ignoreCANPacket();
  }

  return 1;
} 