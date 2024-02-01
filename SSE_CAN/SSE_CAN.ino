#include <CAN.h>
#include <string.h>


/* 
*Starts up CAN communication with a specified baudrate and encapsulates
*console print logging for debugging purposes
*Returns a 0 if the startup failed or a 1 if the startup was successful
*
*@param   baudrate  the baudrate of the CAN communication in bytes per second
*@return            0 or 1 based off if the startup failed or not
*/
void CAN_Startup(long baudrate){
  Serial.println("Attempting to start CAN communications");
  if(!CAN.begin(baudrate)){
    Serial.println("Starting CAN failed");
    return 0;
  }
  return 1;
}

/*
*Transmits a string to a specified id character by character over CAN and encapsulates
*console print logging for debugging purposes
*returns a 1 on completion
*
*@param   id        The CAN id (11 byte limit) that you want to send a message to
*@param   message   The message (no byte limit) you want to transmit    
*@return            a 1 on completion;
*/
void CAN_TransmitString(int id,char *message){
  int messageLength = strlen(message);
  
  Serial.print("Sending message to id: 0x");
  Serial.println(id, HEX);

  CAN.beginPacket(id);

  for(int i = 0; i < messageLength; i++){
    CAN.write(message[i]);
  }

  CAN.endPacket();
  return 1;
}

/*
*Transmits a packet of data to a specified id over CAN and encapsulates
*console print logging for debugging purposes
*returns a 1 on completion
*
*@param   id        The CAN id (11 byte limit) that you want to send a message to
*@param   packet    The message (8 byte limit) you want to transmit    
*@return            a 1 on completion;
*/
void CAN_TransmitPacket(int id,uint8_t packet){
  Serial.print("Sending message to id: ");
  Serial.println(id, HEX);

  CAN.beginPacket(id);

  CAN.write(packet);

  CAN.endPacket();
  return 1;
}

