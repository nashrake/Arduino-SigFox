#include <SigFox.h>

byte digital = 0; // for digital values
byte message[9]; // for writing all pin values 

void setup() {
  Serial.begin( 9600 ); // init serial to 9600b/s
  if (!SigFox.begin()) {
    Serial.println("Shield error or not present!");
    return;
  }
}

  void loop() {
    
    analogWrite(A3, 255); // for the Low Power consumption on Vcc; Writing a3 to high
    
    // to write digital pins values in one byte
    bitWrite(digital, 0, digitalRead(0));
    bitWrite(digital, 1, digitalRead(1));
    bitWrite(digital, 2, digitalRead(2));
    bitWrite(digital, 3, digitalRead(3));
    bitWrite(digital, 4, digitalRead(4));
    bitWrite(digital, 5, digitalRead(5));
    bitWrite(digital, 6, digitalRead(6));
    bitWrite(digital, 7, digitalRead(7));

    delay(1000);

    // to write analog pin values 
    int val = analogRead(A0);
    val = map(val, 0, 1023, 0, 255);
    byte analog = (byte)val;
    
    int val_1 = analogRead(A1);
    val_1 = map(val_1, 0, 1023, 0, 255);
    byte analog_1 = (byte)val_1;
    
    int val_2 = analogRead(A2);
    val_2 = map(val_2, 0, 1023, 0, 255);
    byte analog_2 = (byte)val_2;
    
    int val_3 = analogRead(A3);
    val_3 = map(val_3, 0, 1023, 0, 255);
    byte analog_3 = (byte)val_3;
    
    int val_4 = analogRead(A4);
    val_4 = map(val_4, 0, 1023, 0, 255);
    byte analog_4 = (byte)val_4;
    
    int val_5 = analogRead(A5);
    val_5 = map(val_5, 0, 1023, 0, 255);
    byte analog_5 = (byte)val_5;
    
    int val_6 = analogRead(A6);
    val_6 = map(val_6, 0, 1023, 0, 255);
    byte analog_6 = (byte)val_6;

    // to calculate battery voltage assuming 5.015 v as maximum
    int batVal = analogRead(ADC_BATTERY);
    byte battery = (byte)batVal;

    // to arrange all of the pin values in a byte array
    byte message[9] = { analog, analog_1, analog_2, analog_3, analog_4, analog_5, analog_6, digital, battery };

    // Initializing SigFox
  SigFox.begin();
  //delay(100);

  // Initializing message to send
  SigFox.beginPacket();
  
  // Enabling debug; LED Signaling
  SigFox.debug();

   
 // Sending message with values correspondingly to the limit of 12 bytes

  SigFox.write( message, 9 );

  Serial.print("message: ");
  
  for ( int j=0 ; j<9 ; j++){
  Serial.print(message[j], DEC );
  Serial.print(" ");
  Serial.println( message[j], HEX );
  }
     
  // send buffer to SIGFOX network and wait for a response
  int ret = SigFox.endPacket();  

  Serial.println(ret);
  if (ret > 0) {
    Serial.println("No transmission"); // If endPacket != 0
  } else {
    Serial.println("Transmission ok");// If endPacket = 0
  }

  Serial.println(SigFox.status(SIGFOX));
  Serial.println(SigFox.status(ATMEL));

  SigFox.end();

  analogWrite(A3, 0);// for the Low Power consumption on Vcc; Writing a3 to low

  delay(600000); //Delay of 10 mins as recommended for SigFox protocol

  }
