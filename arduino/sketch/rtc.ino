void setup_rtc(bool rtcSetTime, byte * rtcTime)
{
  Wire.beginTransmission(rtcAddress);
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
  Wire.endTransmission(rtcAddress);
}

void echo_time() 
{ 
  Wire.beginTransmission(rtcAddress);
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
  Wire.endTransmission(rtcAddress);
}