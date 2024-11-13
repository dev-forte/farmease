void setup_rtc(bool rtcSetTime, byte * rtcTime)
{
  // uncomment 'Wire' when using multiple i2c devices
  // Wire.beginTransmission(rtcAddress);
  // to fix time manually
  if (rtcSetTime) {
    myRTC.setClockMode(false);  // 12h(true) 24h(false)
    myRTC.setYear(rtcTime[0]);
    myRTC.setMonth(rtcTime[1]);
    myRTC.setDate(rtcTime[2]);
    myRTC.setHour(rtcTime[3]);
    myRTC.setMinute(rtcTime[4]);
    myRTC.setSecond(rtcTime[5]);
    Serial.println(F("MANUAL TIME SETTING DONE"));
  }
  // Wire.endTransmission(rtcAddress);
}

void echo_time()
{ 
  // Wire.beginTransmission(rtcAddress);
  now = RTClib::now();
    Serial.print(F("RTC TIME: ")); 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);
    Serial.println(F("================="));
  // Wire.endTransmission(rtcAddress);
}

void echo_time2() 
{ 
  // Wire.beginTransmission(rtcAddress);
  now = RTClib::now();
    Serial3.print(F("RTC TIME: ")); 
    Serial3.print(now.year(), DEC);
    Serial3.print('/');
    Serial3.print(now.month(), DEC);
    Serial3.print('/');
    Serial3.print(now.day(), DEC);
    Serial3.print(' ');
    Serial3.print(now.hour(), DEC);
    Serial3.print(':');
    Serial3.print(now.minute(), DEC);
    Serial3.print(':');
    Serial3.println(now.second(), DEC);
    Serial3.println(F("================="));
  // Wire.endTransmission(rtcAddress);
}