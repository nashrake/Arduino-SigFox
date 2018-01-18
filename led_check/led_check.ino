#include <ArduinoLowPower.h>
#include <RTCZero.h>

RTCZero rtc;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 20;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 19;
const byte month = 12;
const byte year = 17;


boolean matched = false;

void setup() {
  pinMode( 7 , OUTPUT);
  
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(16, 21, 00);
  rtc.enableAlarm(rtc.MATCH_MMSS);

  rtc.attachInterrupt(alarmMatch);

  rtc.standbyMode();

}

void alarmMatch(){
  
  matched = true;
  
  }
  
void loop() {
  if(matched){
    digitalWrite( 7, HIGH );
    delay(5000);
    digitalWrite( 7, LOW ); 

    int alarmMinutes = rtc.getMinutes();
    alarmMinutes += 1; // resetting the alarm for the required time interval 
    if (alarmMinutes >= 60) {
      alarmMinutes -= 60;
    }
    
    rtc.setAlarmTime(rtc.getHours(), alarmMinutes, rtc.getSeconds()); //setting reset time for alarm

    rtc.standbyMode();
    }
  else LowPower.sleep();
}
