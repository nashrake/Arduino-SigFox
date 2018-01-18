#include <SigFox.h>

// We want to send a boolean value to signal a binary event
// like open/close or on/off

String value_to_send = "IoT_Factory";

#define DEBUG 1

void setup() {

  if (DEBUG){
    Serial.begin(9600);
    while (!Serial) {};
  }

  // Initialize the SigFox module
  if (!SigFox.begin()) {
    if (DEBUG){
      Serial.println("Sigfox module unavailable !");
    }
    return;
  }

  // If we wanto to debug the application, print the device ID to easily find it in the backend
  if (DEBUG){
    SigFox.debug();
    Serial.println("ID  = " + SigFox.ID());
  }

  delay(100);

  // Compose a message as usual; the single bit transmission will be performed transparently
  // if the data we want to send is suitable
  SigFox.beginPacket();
  SigFox.write(value_to_send);
  int ret = SigFox.endPacket();

  if (DEBUG){
    Serial.print("Status : ");
    Serial.println(ret);
  }
}

void loop(){}
