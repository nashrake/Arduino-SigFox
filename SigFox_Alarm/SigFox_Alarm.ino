#include <SigFox.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial date */

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  rtc.begin();
  
  rtc.setAlarmSeconds(59);
  rtc.enableAlarm(rtc.MATCH_SS);

  rtc.attachInterrupt(alarmMatch);

  rtc.standbyMode();
}

void loop()
{
  rtc.standbyMode();    // Sleep until next alarm match
}

void alarmMatch()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);
}
