#include <SigFox.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>

RTCZero rtc;

const int ANALOGS[] = { A0, A1, A2, A3, A4, A5, A6 };

byte payload[12];
byte payloadLength = 0;

byte buffer = 0;
byte bufferLength = 2;

// Change these values to set the current initial time just for setTime
const byte seconds = 0;
const byte minutes = 35;
const byte hours = 11;

int frequencyLimit = 60; //Setting sending frequency in unit minutes [1 Min <--> 1440 Mins]

boolean matched = false; //boolean for ISR flag

void setup() {

  Serial.begin( 9600 ); // init serial to 9600b/s
  delay(1000);
  //while(!Serial){} //uncomment this line when testing it with Serial Monitor
  pinMode( 7 , OUTPUT);

  if (frequencyLimit < 60 ) {

    rtc.begin();

    rtc.setTime( hours, minutes, seconds ); // Current Time is set as "11:35:00"
    rtc.setAlarmTime( 11, 36, 00 ); //First alarm set to "11:36:00"
    rtc.enableAlarm( rtc.MATCH_MMSS );
    rtc.attachInterrupt( alarmMatch ); //Interrupt to ISR

    rtc.standbyMode();
  }

  else if ( frequencyLimit >= 60 ) {

    rtc.begin();

    rtc.setTime( hours, minutes, seconds ); // Current Time is set as "11:35:00"
    rtc.setAlarmTime( 11, 36, 00 ); // First alarm set to "11:36:00"
    rtc.enableAlarm( rtc.MATCH_HHMMSS );
    rtc.attachInterrupt( alarmMatch ); //Interrupt to ISR

    rtc.standbyMode();
  }

}

void alarmMatch() {

  matched = true; // ISR flag for reading in loop

}

void loop() {

  if ( matched ) {

    digitalWrite( 7 , HIGH ); //For providing Vcc to potentiometer

    matched = false;
    getAnalogs(); // function to get all analog pin readings
    getDigitals(); // function to get all digital pin readings
    getBattery(); // function to get battery reading
    sendPayload(); // function to send the payload
    alarm( frequencyLimit ); // resetting alarm with frequency limit

    digitalWrite( 7, LOW );
  }
  else {
    LowPower.sleep();// Arduino Sleeps if !matched
  }
}

void alarm( int frequency ) {

  if ( frequency < 60 ) {

    // Resetting the alarm for the required time interval if the frequency limit is less than an hour [0 - 59]
    int alarmMinutes = rtc.getMinutes();
    alarmMinutes += frequency;
    rtc.setAlarmTime( rtc.getHours(), alarmMinutes, rtc.getSeconds() ); //resetting time for alarm
    rtc.standbyMode();
  }

  else if ( frequency >= 60 ) {

    //Calculating time from given frequency limit
    int hourAlarm = frequency / 60;
    int alarmHours = rtc.getHours();
    int minutesAlarm = frequency % 60;
    int alarmMinutes = rtc.getMinutes();
    // Resetting the alarm for the required time interval for the frequency limits is or exceeding one hour
    alarmMinutes += minutesAlarm;
    alarmHours += hourAlarm;
    rtc.setAlarmTime( alarmHours, alarmMinutes , rtc.getSeconds() );
    rtc.standbyMode();
  }
}

void getAnalogs() {

  // For each ANALOG input
  for ( int i = 0; i < 7; i++ ) {
    int value = analogRead( ANALOGS[i] );
    addToPayload( value, 10 );

    Serial.print( "Reading Analog" );
    Serial.print( i );
    Serial.print( " : " );
    Serial.println( value );
  }
}

void getDigitals() {

  // For each DIGITAL input
  for ( int i = 0; i < 8; i++ ) {
    boolean value = digitalRead( i );
    addToPayload( value, 1 );

    Serial.print( "Reading Digital" );
    Serial.print( i );
    Serial.print( " : " );
    Serial.println( value );
  }
}

void getBattery() {

  // For battery reading
  int value = analogRead( ADC_BATTERY );
  addToPayload( value, 16 );

  Serial.print( "Reading Battery:" );
  Serial.println( value );
}

void addToPayload( int value, int valueLength ) {

  // To set the values to corresponding bits of payload
  Serial.print( "     Payload : ");

  for ( int i = 0; i < valueLength; i++ ) {

    // Get the value, bit by bit
    int bitPosition = (valueLength - i) - 1;
    boolean bits = value & (1 << bitPosition);

    // Shift buffer
    buffer = buffer << 1;
    bufferLength++;

    // Copy the value into buffer
    if ( bits ) {
      buffer++;
      Serial.print(1);
    } else {
      Serial.print(0);
    }

    // If the buffer is 1byte length
    if ( bufferLength == 8 ) {

      // Copy to payload
      payload[ payloadLength++ ] = buffer;

      // Clean buffer
      buffer = 0;
      bufferLength = 0;
    }
  }
  Serial.println(' ');
}

void sendPayload() {

  // Printing whole message
  for ( int i = 0 ; i < 12 ; i++ ) {
    Serial.print( payload[i], HEX );
  }
  Serial.println(' ');

  // Sending through SigFox
  SigFox.begin();
  SigFox.beginPacket();
  SigFox.debug();
  SigFox.write( payload, 12 );
  int ret = SigFox.endPacket();
  SigFox.end();

  if ( !ret )
    Serial.println("Payload sent through SigFox");
  else
    Serial.println("Payload not sent !");

  payloadLength = 0;
  bufferLength = 2; // For trailing values

}
