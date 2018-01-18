#include "Narcoleptic.h"

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // Enter power down state for 8 s with ADC and BOD module disabled
  longSleep(120000);
  // Do something here
  // Example: Read sensor, data logging, data transmission.
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

}

void longSleep(long milliseconds) 
{ 
   while(milliseconds > 0)
 { 
      if(milliseconds > 8000) 
     { 
            milliseconds -= 8000; Narcoleptic.delay(8000); 
     } 
      else 
     { 
        Narcoleptic.delay(milliseconds); 
        break; 
     } 
 } 
}

