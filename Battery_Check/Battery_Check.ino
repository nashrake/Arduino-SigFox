


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(ADC_BATTERY);
   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 4.3V):
  float voltage = sensorValue;//* (4.3/1023);
  Serial.print(voltage);
  Serial.print( "v" );
  Serial.print("\n");
  delay(10000);
}
