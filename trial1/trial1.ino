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

//byte message[12];
//int messageLength = 0;
long message[12] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
//int count = 0;
 
void setup()
  {
    Serial.begin(9600); // init serial to 9600b/s
    pinMode(analogPin1, OUTPUT); // set ledPin to OUTPUT
  }
  
/*void addBits( value, bitLength ){
  for( int i=0 ; i<bitLength ; i++ ){
        
    buffer = buffer << 1;
    bitWrite(
      buffer,
      0,
      bitRead( x, (bitLength-i)-1 )
    );

    count++;

    if(count == 8){
      message[ messageLength++ ] = buffer;
      count = 0;
      buffer = 0;
    }
        
  }
}*/

/*void value( int val, int bits ){
   
    message = message << bits; // Shifting a byte from 0
  message += val; // Adding the value from the potentiometer 
  //message = (message << 8)+key; // Shifting again a byte and adding key
  }*/

void loop() { 
  
   pin_0 = analogRead( analogPin ); // Reading potentiometer
    if(pin_0 >= 500) digitalWrite(analogPin1,HIGH);  // if adc > 500, led light
    else digitalWrite(analogPin1, LOW);
    Serial.print( pin_0 );
    
   message[1] = analogRead(analogPin1);
   message[2] = analogRead(analogPin2);
   message[3] = analogRead(analogPin3);
   message[4] = analogRead(analogPin4);
   message[5] = analogRead(analogPin5);
   message[6] = analogRead(analogPin6);
   message[7] = digitalRead(digitalPin);
   message[8] = digitalRead(digitalPin1);
   message[9] = digitalRead(digitalPin2);
   message[10] = digitalRead(digitalPin3);
   message[11] = digitalRead(digitalPin4);
   pin_12 = digitalRead(digitalPin5);
   pin_13 = digitalRead(digitalPin6);
   pin_14 = digitalRead(digitalPin7);
   
   int batVal = analogRead( ADC_BATTERY );// to read the volts on battery pin
   float voltage = ( batVal ); // converting sensor value within 0 <--> 255 for precision
   int battery = voltage;
   
   Serial.print(voltage);
   
   SigFox.begin();// Initializing SigFox 
    delay(100);
   SigFox.status();// Clears all pending interrupts
    delay(1);

   SigFox.beginPacket();// Initializing message to send 
   SigFox.debug();// Enabling debug; LED Signaling
   value( pin_0, 10 ); //Call of the function `value` with args of value
   value( pin_1, 10 ); 
   value( pin_2, 10 );
   value( pin_3, 10 );
   value( pin_4, 10 );
   value( pin_5, 10 );
   value( pin_6, 10 );
   value( 0, 2 );
   value( pin_7, 1 );
   value( pin_8, 1 );
   value( pin_9, 1 );
   value( pin_10, 1 );
   value( pin_11, 1 );
   value( pin_12, 1 );
   value( pin_13, 1 );
   value( pin_14, 1 );
   value( battery, 16  ); //Call of the function `value` with args battery value
   
   
   SigFox.write( message[12] ); // Sending message with value correspondingly
   Serial.print( message ); 
   message = 0;// Clearing message value
  
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

