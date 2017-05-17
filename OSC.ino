
void OSCMsgReceive(){
  OSCMessage msgIN;
  int size;
  if((size = Udp.parsePacket())>0){
    while(size--)
      msgIN.fill(Udp.read());
    if(!msgIN.hasError()){
      msgIN.route("/Brightness",funcBright);
      msgIN.route("/Trigger",funcTrigger);
      msgIN.route("/Auto",funcAuto);
    }
  }
}

void funcBright(OSCMessage &msg, int addrOffset ){
  autoDim = 0;
  Serial.println();
  Serial.print("Set Auto: ");
  Serial.println(autoDim);

  Brightness = msg.getFloat(0);
  Serial.print("Brightness: ");
  Serial.println(Brightness);

  String stringOne, stringTwo, iPlayerCommand;
  stringOne = "X02";
  stringTwo = decToHex(map(Brightness, 0, 100, 0, 255), 2);
  stringTwo.toUpperCase();
  iPlayerCommand = stringOne += stringTwo;
  Serial1.print(iPlayerCommand);
  Serial.print("Sent: ");
  Serial.println(iPlayerCommand);

  updateOSC();
//  OSCMessage msgOUT("/Brightness");
//  msgOUT.add(Brightness);
//  Udp.beginPacket(Udp.remoteIP(), destPort);
//  msgOUT.send(Udp); // send the bytes
//  Udp.endPacket(); // mark the end of the OSC Packet
//  msgOUT.empty(); // free space occupied by message
//  
//  OSCMessage msgOUT2("/Brightness/Value");
//  msgOUT2.add(Brightness);
//  Udp.beginPacket(Udp.remoteIP(), destPort);
//  msgOUT2.send(Udp); // send the bytes
//  Udp.endPacket(); // mark the end of the OSC Packet
//  msgOUT2.empty(); // free space occupied by message
//
//  OSCMessage msgOUT3("/Auto");
//  msgOUT3.add(autoDim);
//  Udp.beginPacket(Udp.remoteIP(), destPort);
//  msgOUT3.send(Udp); // send the bytes
//  Udp.endPacket(); // mark the end of the OSC Packet
//  msgOUT3.empty(); // free space occupied by message

}

void funcTrigger(OSCMessage &msg, int addrOffset ){
  Trigger = msg.getFloat(0);
  Serial.println();
  Serial.print("Trigger: ");
  Serial.println(Trigger);

  String stringOne, stringTwo, iPlayerCommand;
  stringOne = "X04";
  stringTwo = decToHex(Trigger, 2);
  stringTwo.toUpperCase();

  iPlayerCommand = stringOne += stringTwo;
  Serial1.print(iPlayerCommand);
  Serial.print("Sent: ");
  Serial.println(iPlayerCommand);
  
  OSCMessage msgOUT("/Trigger/Value");
  msgOUT.add(Trigger);
  Udp.beginPacket(Udp.remoteIP(), destPort);
  msgOUT.send(Udp); // send the bytes
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOUT.empty(); // free space occupied by message
}

void funcAuto(OSCMessage &msg, int addrOffset ){
  autoDim = msg.getFloat(0);
  Serial.print("Auto: ");
  Serial.println(autoDim);

  if(autoDim == 1){
    Brightness = 100-dim;
    //delay (10);
    String stringOne, stringTwo, iPlayerCommand;
    stringOne = "X02";
    stringTwo = decToHex(map(Brightness, 0, 100, 0, 255), 2);
    stringTwo.toUpperCase();
    iPlayerCommand = stringOne += stringTwo;
    Serial1.print(iPlayerCommand);
    Serial.print("Dim Sent: ");
    Serial.print(Brightness);
    Serial.print("%, ");
    Serial.println(iPlayerCommand);
  } 

  updateOSC();
}

void updateOSC(){
      OSCMessage msgOUT("/Brightness");
      msgOUT.add(Brightness);
      Udp.beginPacket(Udp.remoteIP(), destPort);
      msgOUT.send(Udp); // send the bytes
      Udp.endPacket(); // mark the end of the OSC Packet
      msgOUT.empty(); // free space occupied by message
      
      OSCMessage msgOUT2("/Brightness/Value");
      msgOUT2.add(Brightness);
      Udp.beginPacket(Udp.remoteIP(), destPort);
      msgOUT2.send(Udp); // send the bytes
      Udp.endPacket(); // mark the end of the OSC Packet
      msgOUT2.empty(); // free space occupied by message
    
      OSCMessage msgOUT3("/Auto");
      msgOUT3.add(autoDim);
      Udp.beginPacket(Udp.remoteIP(), destPort);
      msgOUT3.send(Udp); // send the bytes
      Udp.endPacket(); // mark the end of the OSC Packet
      msgOUT3.empty(); // free space occupied by message
}

