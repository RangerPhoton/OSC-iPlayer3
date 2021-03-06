
//Set up Ethernet and OSC 
//adapted from https://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/)
//#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <OSCBundle.h>
#include <elapsedMillis.h>

// you can find this written on the board of some Arduino Ethernets or shields
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 

// NOTE: Alternatively, you can assign a fixed IP to configure your
//       Ethernet shield.
       byte ip[] = { 192, 168, 0, 101 };

int serverPort  = 9998; //TouchOSC's "outgoing port"
int destPort = 9999;    //TouchOSC's "incoming port"

//Create UDP message object
EthernetUDP Udp;

//int incomingByte = 0;   // for incoming serial data

elapsedMillis mils; //millis count up to 999 then increment secs
int secs = 0; // secs count up to 59 then increment mins
int mins = 0; // resulting in mins since power on
int dim = 0; //dimming value is incremented once each min
int dimDelay = 1; // delay before starting to increment dim
int autoDim = 1; // dim is subtracted from brightness when autoDim = 1)

int Brightness = 100; // Brightness value sent to iPlayer3, controlled by autoDim and OSC fader
int Trigger = 0; // show 

void setup()
{  
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);    // begin reset the WIZ820io
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);  // de-select WIZ820io
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);   // de-select the SD Card
  digitalWrite(9, HIGH);   // end reset pulse
  
  Serial.begin(9600); //9600 for a "normal" Arduino board (Uno for example). 115200 for a Teensy ++2 
  Serial.println("OSC test");

  Serial1.begin(9600); //9600 for a "normal" Arduino board (Uno for example). 115200 for a Teensy ++2 

  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);

  // start the Ethernet connection:
  // NOTE: Alternatively, you can assign a fixed IP to configure your
  //       Ethernet shield.
         Ethernet.begin(mac, ip);   
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // no point in carrying on, so do nothing forevermore:
//    while(true);
//  }
  // print your local IP address:
  Serial.print("Arduino IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    if(thisByte < 3) Serial.print("."); 
    else Serial.println("");
  }

  Udp.begin(serverPort);

//  delay (200); //Allow time for OSC connection
//  updateOSC();

}

void loop()
{
  OSCMsgReceive();

  if (mils > 999){
    secs += 1;
    mils = 0;
    //Serial.print("-");
    if (secs > 59){
      mins += 1;
      secs = 0;
      Serial.println();
      Serial.print("Min:"); 
      Serial.print(mins);
      Serial.print(", Dim:"); 
      Serial.print(dim);
      Serial.print(", Auto:"); 
      Serial.println(autoDim); 
       
      if (mins >= dimDelay){
        dim = constrain(mins-dimDelay+1, 0, 40);  // Dim down to 60% brightness then stop (Title 12)
        if(autoDim == 1){
          Brightness = 100-dim;
          //delay (10);
          String stringOne, stringTwo, iPlayerCommand;
          stringOne = "X02";
          stringTwo = decToHex(map(Brightness, 0, 100, 0, 255), 2);
          stringTwo.toUpperCase();
          iPlayerCommand = stringOne += stringTwo;
          Serial1.print(iPlayerCommand);
          Serial.print("Sent: ");
          Serial.print(Brightness);
          Serial.print("%, ");
          Serial.println(iPlayerCommand);
        }      
      }
    }
    updateOSC();

  }

//  if (stringComplete) {
//    Serial.print("Received: ");
//    Serial.println(inputString);
//    // clear the string:
//    inputString = "";
//    stringComplete = false;
//  }



//    if (Serial1.available() > 0) {
//            // read the incoming byte:
//            incomingByte = char(Serial1.read());
//
//            // say what you got:
//            Serial.print("Received: ");
//            Serial.write(incomingByte);
//            Serial.println();
//    }
    
}

