/*#include "SSE_CAN.h"

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
*/
// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Receiver");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(1000E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = CAN.parsePacket();

  if (packetSize || CAN.packetId() != -1) {
    // received a packet
    Serial.print("Received ");

    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }

    if (CAN.packetRtr()) {
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());
    } else {
      Serial.print(" and length ");
      Serial.println(packetSize);

      // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print((int)CAN.read());
        Serial.print(" ");
      }
      Serial.println();
    }

    Serial.println();
  }
}