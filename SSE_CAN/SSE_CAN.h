#include <CAN.h>
#include <string.h>
#include "SSE_PacketTranslator.h"

void CAN_Scanner(int);
void CAN_LogPacket();


/* 
*Starts up CAN communication with a specified baudrate and encapsulates
*console print logging for debugging purposes.
*Returns a 0 if the startup failed or a 1 if the startup was successful
*
*@param   baudrate  the baudrate of the CAN communication in bytes per second
*@return            0 or 1 based off if the startup failed or not
*/
int CAN_Startup(long baudrate){
  Serial.println("Attempting to start CAN communications");
  if(!CAN.begin(baudrate)){
    Serial.println("Starting CAN failed");
    return 0;
  }
  else{
    Serial.println("Starting CAN WORKED!!!!!!!");
  }
  //this checks for ANY packets on the CAN bus then calls CAN_Scanner function with the length of the packet as the input
  CAN.onReceive(CAN_Scanner);
  return 1;
}

/*
*!YOU SHOULD NOT CALL THIS!.
*
*gets called everytime a packet gets recieved.
*prints out the ID of the device that sent the packet and the length of the packet it recieved
*then calls CAN_LogPacket().
*
*@param packetLength  the packet length of the packet that was just recieved
*/
void CAN_Scanner(int packetLength){
  Serial.print("Recieved a packet from the id: 0x");
  Serial.println(CAN.packetId(), HEX);
  Serial.print("Packet has a byte length of: ");
  Serial.println(packetLength);

  CAN_LogPacket();
}

/*
*WARNING: expect this to produce a nonhuman readable output.
*
*prints out a recieved packet byte by byte, separated by a singular space, formatted in hexadecimal format
*/
void CAN_LogPacket(){
  Serial.println("Packet contents:");
  for (int i = 0; i < CAN.packetDlc(); i++){
    Serial.print(CAN.read(), HEX);
    Serial.print(" ");
  }
  Serial.println();
}

/*
*Transmits a string to a specified id character by character over CAN and encapsulates
*console print logging for debugging purposes.
*Returns a 1 on completion
*
*@param   id        The CAN id (11 byte limit) that you want to send a message to
*@param   message   The message (8 byte limit) you want to transmit    
*@return            a 1 on completion;
*/
int CAN_TransmitString(int id,char *message){
  int messageLength = strlen(message);
  
  Serial.print("Sending message to id: 0x");
  Serial.println(id, HEX);

  CAN.beginPacket(id);

  CAN.write(message,messageLength);

  CAN.endPacket();
  return 1;
}

/*
*Transmits a packet of data to a specified id over CAN and encapsulates
*console print logging for debugging purposes.
*Returns a 1 on completion
*
*@param   id        The CAN id (11 byte limit) that you want to send a message to
*@param   packet    The message (1 byte limit) you want to transmit    
*@return            a 1 on completion;
*/
int CAN_TransmitPacket(int id,uint8_t packet){
  Serial.print("Sending packet to id: ");
  Serial.println(id, HEX);

  CAN.beginPacket(id);

  CAN.write(packet);

  CAN.endPacket();

  Serial.println("Packet has been sent");
  return 1;
}



