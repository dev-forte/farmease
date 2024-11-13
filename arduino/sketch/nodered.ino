void from_nodered()
{ 
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  if (Serial3.available()){
    char command_serial = Serial3.read();
    
    // pump manual on/off
    if ( command_serial == 'p' ) {
      Serial.print(F("Node-RED "));
      pump_on();
      message += "manual pump on";
      publish_actuator(message);
      echo_time();
    }
    else if (command_serial == 'q') {
      Serial.print(F("Node-RED "));
      pump_off();
      message += "manual pump off";
      publish_actuator(message);
      echo_time();
    }

    // led manual on/off
    else if ( command_serial == 'l' ) {
      Serial.print(F("Node-RED "));
      led_on();
      message += "manual led on";
      publish_actuator(message);
      echo_time();
    }
    else if (command_serial == 'm') {
      Serial.print(F("Node-RED "));
      led_off();
      message += "manual led off";
      publish_actuator(message);
      echo_time();
    }

    // sensor measurement and publishing
    else if ( command_serial == 's' ) {
      Serial.println(F("Node-RED "));
      sensor();
      echo_time();
    }
    else if (command_serial == 't') {
      Serial.print(F("Node-RED "));
      publish_sensor();
      echo_time();
    }

    else if ( command_serial == 'd' ) {
      Serial.println(F("Set Nutirent Solution Dose Per Hour"));
      Serial.println(F("(from 0 to 200 in ml): "));
      int temporary_dose = Serial3.parseInt();
      if (temporary_dose < 0) {
        dose = 0;
      }
      else if (temporary_dose > 200) {
        dose = 200;
      }
      else {
        dose = temporary_dose;
      }
      dose_to_time(dose);
      Serial.print(F("New Dose: "));
      Serial.print(dose);
      Serial.println(F(" Per Hour"));
      echo_time();
    }
    ClearBuffer();
  }
}

void ClearBuffer(){
  while(Serial3.available() > 0) {
    Serial3.read();
  }
}

