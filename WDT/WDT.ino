#include <Adafruit_SleepyDog.h>
#include <SigFox.h>

const int ANALOGS[] = { A0, A1, A2, A3, A4, A5, A6 };

byte payload[12];
byte payloadLength = 0;

byte buffer = 0;
byte bufferLength = 2;

void setup(){
    Serial.begin( 9600 ); // init serial to 9600b/s
}

void loop() {

    getAnalogs();
    
    getDigitals();
    
    getBattery();
    
    sendPayload();
    
    Watchdog.sleep( 120000 ); 

}

void getAnalogs(){
    // For each ANALOG input
    for( int i = 0; i < 7; i++ ) {
        int value = analogRead( ANALOGS[i] );
        addToPayload( value, 10 );

        Serial.print( "Reading Analog" );
        Serial.print( i );
        Serial.print( " : " );
        Serial.println( value );
    }
}

void getDigitals(){
    // For each DIGITAL input
    for( int i = 0; i < 8; i++ ) {
        boolean value = digitalRead( i );
        addToPayload( value, 1 );

        Serial.print( "Reading Digital" );
        Serial.print( i );
        Serial.print( " : " );
        Serial.println( value );
    }
}

void getBattery(){
    int value = analogRead( ADC_BATTERY );
    addToPayload( value, 16 );

    Serial.print( "Reading Battery:" );
    Serial.println( value );
}

void addToPayload( int value, int valueLength ){

    Serial.print( "     Payload : ");

    for( int i = 0; i < valueLength; i++ ) {

        // Get the value, bit by bit
        int bitPosition = (valueLength-i)-1;
        boolean bits = value & (1 << bitPosition);

        // Shift buffer
        buffer = buffer << 1;
        bufferLength++;

        // Copy the value into buffer
        if( bits ){
            buffer++;
            Serial.print(1);
        }else{
            Serial.print(0);
        }

        // If the buffer is 1byte length
        if( bufferLength == 8 ){

            // Copy to payload
            payload[ payloadLength++ ] = buffer;

            // Clean buffer
            buffer = 0;
            bufferLength = 0;

        }

    }

    Serial.println(' ');

}

void sendPayload(){

    // Printing whole message
    for( int i = 0 ; i < 12 ; i++ ){
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

    if( !ret )
        Serial.println("Payload sent through SigFox");
    else
        Serial.println("Payload not sent !");
        
    payloadLength = 0;
    bufferLength = 2;

}


