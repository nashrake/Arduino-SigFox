#include <SigFox.h>
#include <ArduinoLowPower.h>
void setup() {
 Serial.begin(9600);
 while (!Serial) {};
if (!SigFox.begin()) {
 Serial.println("Shield error or not present!");
 return;
 }
 // Enable debug led and disable automatic deep sleep
 // Comment this line when shipping your project :)
 SigFox.debug();
String version = SigFox.SigVersion();
 String ID = SigFox.ID();
 String PAC = SigFox.PAC();
// Display module informations
 Serial.println("MKRFox1200 Sigfox first configuration");
 Serial.println("SigFox FW version "+ version);
 Serial.println("ID = "+ ID);
 Serial.println("PAC = "+ PAC);
Serial.println("");
Serial.println("Register your board on https://backend.sigfox.com/activate with provided ID and PAC");
delay(100);
// Send the module to the deepest sleep
 SigFox.end();
Serial.println("Type the message to be sent");
 while (!Serial.available());
String message;
 while (Serial.available()) {
 message += (char)Serial.read();
 }
// Every SigFox packet cannot exceed 12 bytes
 // If the string is longer, only the first 12 bytes will be sent
if (message.length() > 12) {
 Serial.println("Message too long, only first 12 bytes will be sent");
 }
Serial.println("Sending "+ message);
// Remove EOL
 message.trim();
// Example of message that can be sent
 sendString(message);
 
}
void loop()
{
}
void sendString(String str) {
 // Start the module
 SigFox.begin();
 // Wait at least 30mS after first configuration (100mS before)
 delay(100);
 // Clears all pending interrupts
 SigFox.status();
 delay(1);
SigFox.beginPacket();
 SigFox.print(str);
int ret = SigFox.endPacket(); // send buffer to SIGFOX network
 if (ret > 0) {
 Serial.println("No transmission");
 } else {
 Serial.println("Transmission ok");
 }
 Serial.println(SigFox.status(SIGFOX));
 Serial.println(SigFox.status(ATMEL));
 SigFox.end();
}
