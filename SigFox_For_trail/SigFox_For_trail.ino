#include <SigFox.h>
#include "ArduinoLowPower.h"
#define SleepTime 2*60*1000

void setup() {
  Serial.begin(9600);
    if (!SigFox.begin()) {
    Serial.println("Shield error or not present!");
    return;
  }
}

void loop() {
  SigFox.debug();
  SigFox.begin();
  delay(100);
  String msg= "Check1";
  int flag = SigFox.beginPacket();
  Serial.print("flag : ");
  Serial.println(flag);
  msg.trim();
  SigFox.write(msg);
  Serial.println(msg);
 
  int ret = SigFox.endPacket();  // send buffer to SIGFOX network
  if (ret > 0) {
    Serial.print("No transmission: ");
    Serial.println(ret);
  } else {
    Serial.print("Transmission ok: ");
    Serial.println(ret);
    }
    SigFox.end();
    LowPower.sleep(SleepTime);
}

