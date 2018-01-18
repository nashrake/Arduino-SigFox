/*Crack Metering 

To collect the input of the device connected to the Arduino MKRFOX1200 and manipulate those data within 12 Bytes to send it to SigFox backend

Potentiometer device input is taken in this case, connected to the pin A0

Main library used is <SigFox.h>*/


#include <SigFox.h>


byte message[12]; //variable to send 12 byte of data to SigFox backend
int messageLength = 0;
byte buffer = 0;
int bufferLength = 0;


//Setting up Serial and pinModes as required
void setup()
{
  Serial.begin( 9600 ); // init serial to 9600b/s
  pinMode( A1, OUTPUT ); // set ledPin to OUTPUT
}


// function to write the values bit by bit within 12 byte of message
void addBits( int value, int bitLength ) {

  // Bit by bit
  for ( int i = 0 ; i < bitLength ; i++ ) {

    //Shift the buffer
    buffer = buffer << 1;

    // Copy the bit to the buffer
    if ( value & (1<<((bitLength-i)-1)) ){
        Serial.print("1");
        buffer++;
    }else{
        Serial.print("0");
    }

    // Increase the bufferLength
    bufferLength++;

    // If the bufferLength is one byte
    if ( bufferLength == 8 ) {
      // Store it into the message
      message[ messageLength ] = buffer;
      messageLength++;

      // Clear the buffer & bufferLength
      bufferLength = 0;
      buffer = 0;
    }     
  }
  Serial.print(" ");
  Serial.println(value);
}

//funtion to run the program over and again with the given time limit
void loop() {

  Serial.print( "Potentiometer Value: " );
  Serial.print( analogRead( A0 ) );
  Serial.print( "\n" );
  if (analogRead( A0 ) >= 500) digitalWrite( A1, HIGH ); // if pin_0 > 500, light LED of Potentiometer
  else digitalWrite( A1, LOW );
 
  // to read the volts on battery pin
  int batVal = analogRead( ADC_BATTERY );
  float voltage =  batVal;
  long battery = voltage;

  // Initializing SigFox
  SigFox.begin();
  //delay(100);

  // Initializing message to send
  SigFox.beginPacket();
  
  // Enabling debug; LED Signaling
  SigFox.debug();
  
  //Call of the function `value` with args of value for analog pins
  addBits( 0, 2 );
  addBits( analogRead( A0 ), 10 ); 
  addBits( analogRead( A1 ), 10 );
  addBits( analogRead( A2 ), 10 );
  addBits( analogRead( A3 ), 10 );
  addBits( analogRead( A4 ), 10 );
  addBits( analogRead( A5 ), 10 );
  addBits( analogRead( A6 ), 10 );
  //addBits( 0, 2 ); // Separator
  addBits( digitalRead( 0 ), 1 );//Call of the function `value` with args of value for digital pins
  addBits( digitalRead( 1 ), 1 );
  addBits( digitalRead( 2 ), 1 );
  addBits( digitalRead( 3 ), 1 );
  addBits( digitalRead( 4 ), 1 );
  addBits( digitalRead( 5 ), 1 );
  addBits( digitalRead( 6 ), 1 );
  addBits( digitalRead( 7 ), 1 );
  addBits( battery, 16 );
  
 // Sending message with values correspondingly to the limit of 12 bytes
  SigFox.write( message,12 );
  Serial.print("message: ");
  
  for ( int j=0 ; j<12 ; j++){
  Serial.print(message[j], BIN );
  Serial.println( message[j], HEX );
  }
  // Clearing message value
  messageLength = 0; 
  
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
  

  delay(120000);// Delay for 10 mins; limiting the no. of messages for SigFox transmission protocol
}

