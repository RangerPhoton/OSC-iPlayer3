/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

//String inputString = "";         // a string to hold incoming data
//boolean stringComplete = false;  // whether the string is complete
 
//void serialEvent1() {
//  int count = 0;
//  while (Serial1.available()) {
//    // get the new bytes:
//    for (count = 0; count < 5; count++) {
//    char inChar = char(Serial1.read());
//    // add it to the inputString:
//      if (inChar == 'X' || inChar == 'Y') count = 0; //
//      inputString += inChar;
//      if (count == 4) stringComplete = true;
//    }
//  }
//}


// Converting from Hex to Decimal:

// NOTE: This function can handle a positive hex value from 0 - 65,535 (a four digit hex string).
//       For larger/longer values, change "unsigned int" to "long" in both places.
// https://github.com/benrugg/Arduino-Hex-Decimal-Conversion/blob/master/hex_dec.ino

unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}


// Converting from Decimal to Hex:

// NOTE: This function can handle a positive decimal value from 0 - 255, and it will pad it
//       with 0's (on the left) if it is less than the desired string length.
//       For larger/longer values, change "byte" to "unsigned int" or "long" for the decValue parameter.

String decToHex(byte decValue, byte desiredStringLength) {
  
  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength) hexString = "0" + hexString;
  
  return hexString;
}


 // ****** UTF8-Decoder: convert UTF8-string to extended ASCII *******
static byte c1;  // Last character buffer

// Convert a single Character from UTF8 to Extended ASCII
// Return "0" if a byte has to be ignored
byte utf8ascii(byte ascii) {
    if ( ascii<128 )   // Standard ASCII-set 0..0x7F handling  
    {   c1=0; 
        return( ascii ); 
    }

    // get previous input
    byte last = c1;   // get last char
    c1=ascii;         // remember actual character

    switch (last)     // conversion depending on first UTF8-character
    {   case 0xC2: return  (ascii);  break;
        case 0xC3: return  (ascii | 0xC0);  break;
        case 0x82: if(ascii==0xAC) return(0x80);       // special case Euro-symbol
    }

    return  (0);                                     // otherwise: return zero, if character has to be ignored
}


// convert String object from UTF8 String to Extended ASCII
String utf8ascii(String s)
{  
  String r="";
  char c;
  for (int i=0; i<s.length(); i++)
  {
    c = utf8ascii(s.charAt(i));
    if (c!=0) r+=c;
  }
  return r;
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
void utf8ascii(char* s)
{ 
  int k=0;
  char c;
  for (int i=0; i<strlen(s); i++)
  {
    c = utf8ascii(s[i]);
    if (c!=0) 
      s[k++]=c;
  }
  s[k]=0;
}
