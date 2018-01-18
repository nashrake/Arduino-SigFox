#include<SigFox.h>

void setup() {
  Serial.begin(9600); 
  while(!Serial)  {}; //waits for Serial to be available
  if (!SigFox.begin()) { //ensures SigFox is connected
    Serial.println("Shield error or not present!");
    return;
  }

  // start the module
  SigFox.begin();
  //waiting time 
  delay(100);
  //Clears all pending interrupts
  SigFox.status();
  delay(1);

  SigFox.beginPacket();
  SigFox.print("Hello");

  int ret = SigFox.endPacket();
  if (ret > 0){
    Serial.println("No Transmission");
    } else {
    Serial.println("Transmission OK");
    }

    Serial.println(SigFox.status(SIGFOX));
    Serial.println(SigFox.status(ATMEL));
    SigFox.end();

}
void loop() {
  while(1);
}


