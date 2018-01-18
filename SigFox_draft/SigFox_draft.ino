#include <SigFox.h>

// Defining Analog pins A0 <-> A6
int analogPin = A0; // Input pin for the potentiometer
int analogPin1 = A1; // Pin for the LED
int analogPin2 = A2;
int analogPin3 = A3;
int analogPin4 = A4;
int analogPin5 = A5;
int analogPin6 = A6;

//Defining Digital pins 0 <-> 7
int digitalPin = 0;
int digitalPin1 = 1;
int digitalPin2 = 2;
int digitalPin3 = 3;
int digitalPin4 = 4;
int digitalPin5 = 5;
int digitalPin6 = 6;
int digitalPin7 = 7;

// Defining variables to store value of each pin
int pin_0 = 0;   // Variable to store the value coming from the sensor
int pin_1 = 0;   // Variable to store LED value
int pin_2 = 0;
int pin_3 = 0;
int pin_4 = 0;
int pin_5 = 0;
int pin_6 = 0;
int pin_7 = 0;
int pin_8 = 0;
int pin_9 = 0;
int pin_10 = 0;
int pin_11 = 0;
int pin_12 = 0;
int pin_13 = 0;
int pin_14 = 0;

byte message[12]; //variable to send 12 byte of data to SigFox backend
int messageLength = 0;
byte buffer = 0;
int count = 0;


//Setting up Serial and pinModes as required
void setup()
{
  Serial.begin( 9600 ); // init serial to 9600b/s
  pinMode( analogPin1, OUTPUT ); // set ledPin to OUTPUT
  pinMode(analogPin, INPUT);
}


// function to write the values bit by bit within 12 byte of message
void addBits( byte value, int bitLength ) {
  for ( int i = 0 ; i < bitLength ; i++ ) {

    buffer = buffer << 1; //shifting a bit for storing value
    bitWrite( //Writing the value on the specified bit; bitWrite(x, n, b);
      buffer, //x=> numeric variable to write; n=> Which no.of bit to write from 0 for LSB; b=> value to write for the bit 0 || 1;
      0,
      bitRead( value, (bitLength - i) - 1 )
    );

    count++;

    if ( count == 8 ) {
      message[ messageLength ] = buffer;
      messageLength++;
      count = 0;
      buffer = 0;
    }
  }
}
void value (int value, int place){
  
  message = message << place;
  message += value;
 
 }



//funtion to run the program over and again with the given time limit
void loop() {

  pin_0 = analogRead( analogPin ); // Reading potentiometer
  Serial.print( "Potentiometer Value: " );
  Serial.print( pin_0 );
  Serial.print( "\n" );
  if (pin_0 >= 500) digitalWrite( analogPin1, HIGH ); // if pin_0 > 500, light LED of Potentiometer
  else digitalWrite( analogPin1, LOW );

  pin_1 = analogRead( analogPin1 ); //Read and store analog & digital pins to a variable
  pin_2 = analogRead( analogPin2 );
  pin_3 = analogRead( analogPin3 );
  pin_4 = analogRead( analogPin4 );
  pin_5 = analogRead( analogPin5 );
  pin_6 = analogRead( analogPin6 );
  pin_7 = digitalRead( digitalPin );
  pin_8 = digitalRead( digitalPin1 );
  pin_9 = digitalRead( digitalPin2 );
  pin_10 = digitalRead( digitalPin3 );
  pin_11 = digitalRead( digitalPin4 );
  pin_12 = digitalRead( digitalPin5 );
  pin_13 = digitalRead( digitalPin6 );
  pin_14 = digitalRead( digitalPin7 );

  int batVal = analogRead( ADC_BATTERY );// to read the volts on battery pin
  float voltage =  batVal;
  long battery = voltage;
  
  SigFox.begin();// Initializing SigFox
  delay(100);
  SigFox.status();// Clears all pending interrupts
  delay(1);

  SigFox.beginPacket();// Initializing message to send
  SigFox.debug();
  
  // Enabling debug; LED Signaling
  value( pin_0, 10 ); //Call of the function `value` with args of value for analog pins
  value( pin_1, 10 );
  value( pin_2, 10 );
  value( pin_3, 10 );
  value( pin_4, 10 );
  value( pin_5, 10 );
  value( pin_6, 10 );
  value( 0, 2 ); // Separator
  value( pin_7, 1 );//Call of the function `value` with args of value for digital pins
  value( pin_8, 1 );
  value( pin_9, 1 );
  value( pin_10, 1 );
  value( pin_11, 1 );
  value( pin_12, 1 );
  value( pin_13, 1 );
  value( pin_14, 1 );
  value( battery, 16 );
  */
 // Sending message with values correspondingly
  SigFox.write( message, 12 );
  Serial.print("message: ");
  Serial.print( message[12] );
   
  
  messageLength = 0; // Clearing message value
  int ret = SigFox.endPacket();  // send buffer to SIGFOX network and wait for a response

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

