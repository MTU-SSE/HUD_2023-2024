#include "SSE_CAN.h"

int can_data;

void setup(){
  Serial.begin(9600);

  CAN_Startup(125E3);

}


void loop(){
    //CAN.onReceive(CAN_Scanner);
    //CAN_TransmitPacket(0x123,'8');
    //Serial.print("Packet segment content: 0x");
    
    can_data = CAN.read();
    if (can_data != -1) {
      Serial.print("n");
    }
    
    
    //Serial.println(CAN.read(),HEX);
}