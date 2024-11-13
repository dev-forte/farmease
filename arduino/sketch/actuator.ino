#define pump_channel 0x0000
#define led1_channel 0x0005
#define led2_channel 0x0003


// change schedule 
void dose_to_time(int input_dose) 
{
  int minutes, seconds; 
  float consumption_per_minute = 2000.0 / 60.0;  // dose per minute (33.33ml)
  float time_in_minutes = input_dose / consumption_per_minute;

  minutes = (int)time_in_minutes;
  seconds = (int)((time_in_minutes - minutes) * 60);
  if (seconds < 5) {
    seconds = 5;
  }
  pump_time[3] = minutes;
  pump_time[4] = seconds;
}

void scheduled_pump(int hour, int min, int sec) 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  bool pump_flag_now = false;
  if ( pump_time[0] <= hour && hour <= pump_time[1] ) {
    if ( pump_time[2] <= min && min < pump_time[2] + pump_time[3] ) {
      pump_flag_now = true;
    }
    else if (min == pump_time[2] + pump_time[3] && sec < pump_time[4]) {
      pump_flag_now = true;
    }
    else {
      pump_flag_now = false;
    }
  }

  pump_check(pump_flag_now);

  // scheduled ON
  if (pump_flag_now && !pump_flag)
  {
    pump_on();
    echo_time();
    pump_flag = true;
    pump_check_after(true);
    message += "scheduled pump on";
    publish_actuator(message);
  }
  // scheduled OFF
  else if (!pump_flag_now && pump_flag)
  {
    pump_off();
    echo_time();
    pump_flag = false;
    pump_check_after(false);
    message += "scheduled pump off ";
    message += String(pump_time[3]*60 + pump_time[4])+"(s)";
    publish_actuator(message);
  }
}

void scheduled_led(int hour, int min, int sec)
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  bool led_flag_now = false;
  if ( led_time[0] <= hour && hour < led_time[1] ) { 
    led_flag_now = true;
  }

   led_check(led_flag_now);
  
  // scheduled ON
  if (led_flag_now && !led_flag)
  {
    led_on();
    echo_time();
    led_flag = true;
    message += "scheduled led on";
    publish_actuator(message);
  }
  // scheduled OFF
  else if (!led_flag_now && led_flag)
  {
    led_off();
    echo_time();
    led_flag = false;
    message += "scheduled led off";
    publish_actuator(message);
  }
}

void pump_on() 
{
  delay(20);
  modbus.writeSingleCoil( relay_ID, pump_channel, true );
  Serial.println(F("Pump Start"));
}

void pump_off() 
{
  delay(20);
  modbus.writeSingleCoil( relay_ID, pump_channel, false );
  Serial.println(F("Pump Stop"));
}

void led_on() 
{
  delay(20);
  modbus.writeSingleCoil( relay_ID, led1_channel, true );
  modbus.writeSingleCoil( relay_ID, led2_channel, true );
  Serial.println(F("LED Start"));
}

void led_off() 
{
  delay(20);
  modbus.writeSingleCoil( relay_ID, led1_channel, false );
  modbus.writeSingleCoil( relay_ID, led2_channel, false );
  Serial.println(F("LED Stop"));
}

// check if current pump state is on schedule
// (현재 펌프 상태와 스케줄을 비교합니다.)
void pump_check(bool pump_flag_now) 
{
  delay(20);
  modbus.readCoils( relay_ID, pump_channel, relay_state, 8 );
  bool pump_state = relay_state[pump_channel];
  if ( pump_flag != pump_state ) {
    if (pump_state == true) {
      Serial.println(F("PUMP ERROR: UNEXPECTED ON"));
    }
    else {
      Serial.println(F("PUMP ERROR: UNEXPECTED OFF"));
    }
  }
}

void pump_check_after(bool pump_command) 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  delay(20);
  modbus.readCoils( relay_ID, pump_channel, relay_state, 8 );
  bool pump_state = relay_state[pump_channel];
  if ( pump_command - pump_state ) {
    if (pump_state == true) {
      Serial.println(F("PUMP ERROR: OFF COMMAND FAILED"));
      Serial.println(F("TURNING OFF PUMP IN 2 SECONDS..."));
      delay(1900);
      pump_off();
      echo_time();
      pump_flag = false;
      message += "scheduled pump off (delayed)";
      publish_actuator(message);
    }
    else {
      Serial.println(F("PUMP ERROR: ON COMMAND FAILED"));
      Serial.println(F("TURNING ON PUMP IN 2 SECONDS..."));
      delay(1900);
      pump_on();
      echo_time();
      pump_flag = true;
      message += "scheduled pump on (delayed)";
      publish_actuator(message);
    }
  }
}

// on failured led schedule
// (현재 LED 상태와 스케줄을 비교하여, 다를 경우 자동으로 켜고 끕니다.)
void led_check(bool led_flag_now) 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  delay(20);
  // modbus.readCoils( relay_ID, 0x0000, relayState, 8 );
  bool led_state = relay_state[5];
  if ( led_flag != led_state ) {
    if (led_state == true) {
      Serial.println(F("LED ERROR: UNEXPECTED ON"));
      Serial.println(F("TURNING OFF LED IN 2 SECONDS..."));
      delay(1900);
      led_off();
      echo_time();
      led_flag = false;
      message += "scheduled led off (delayed)";
      publish_actuator(message);
    }
    else {
      Serial.println(F("LED ERROR: UNEXPECTED OFF"));
      Serial.println(F("TURNING ON LED IN 2 SECONDS..."));
      delay(1900);
      led_on();
      echo_time();
      led_flag = true;
      message += "scheduled led on (delayed)";
      publish_actuator(message);
    }
  }
}
