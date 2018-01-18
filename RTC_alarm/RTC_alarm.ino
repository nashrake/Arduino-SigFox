#include <SigFox.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>


RTCZero rtc;

const int ANALOGS[] = { A0, A1, A2, A3, A4, A5, A6 };

byte payload[12];
byte payloadLength = 0;

byte buffer = 0;
byte bufferLength = 2;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 35;
const byte hours = 11;

/* Change these values to set the current initial date */
const byte day = 18;
const byte month = 12;
const byte year = 17;

boolean matched = false;


void setup() {
  Serial.begin( 9600 ); // init serial to 9600b/s

  pinMode( 6 , OUTPUT);

  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(11, 36, 00);
  rtc.enableAlarm(rtc.MATCH_MMSS);

  rtc.attachInterrupt(alarmMatch);

  rtc.standbyMode();
}


void alarmMatch() {

  matched = true;
}

void loop() {

  if (matched) {

    digitalWrite( 6 , HIGH );

    matched = false;

    getAnalogs(); // function to get all analog pin readings

    getDigitals(); // function to get all digital pin readings

    getBattery(); // function to get battery reading

    sendPayload(); // function to send the payload

    int alarmMinutes = rtc.getMinutes();
    alarmMinutes += 1; // resetting the alarm for the required time interval
    if (alarmMinutes >= 60) {
      alarmMinutes -= 60;
    }

    digitalWrite( 6, LOW );
    rtc.setAlarmTime(rtc.getHours(), alarmMinutes, rtc.getSeconds()); //setting reset time for alarm

    rtc.standbyMode();

  }
  else {
    LowPower.sleep();
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
  // To set the values corresponding to the payload

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
  bufferLength = 2;

}
