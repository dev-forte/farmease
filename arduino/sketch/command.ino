void serialEvent() 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  char command_serial = Serial.read();
  // pump manual on/off
  if ( command_serial == '1' ) {
    Serial.print(F("Manual "));
    pump_on();
    message += "manual pump on";
    publish_actuator(message);
    echo_time();
  }
  else if (command_serial == '2') {
    Serial.print(F("Manual "));
    pump_off();
    message += "manual pump off";
    publish_actuator(message);
    echo_time();
  }

  // led manual on/off
  else if ( command_serial == '3' ) {
    Serial.print(F("Manual "));
    led_on();
    message += "manual led on";
    publish_actuator(message);
    echo_time();
  }
  else if (command_serial == '4') {
    Serial.print(F("Manual "));
    led_off();
    message += "manual led off";
    publish_actuator(message);
    echo_time();
  }

  // get status report manually
  else if ( command_serial == 'r' ) {
    // LED state
    Serial.print(F("LED state: "));
    if ( relay_state[5] == true ){
      Serial.print(F("ON"));
    }
    else {
      Serial.print(F("OFF"));
    }
    Serial.print(F("\t\t"));

    // Pump state
    Serial.print(F("Pump state: "));
    if ( relay_state[0] == true ) {
      Serial.print(F("ON"));
    }
    else {
      Serial.print(F("OFF"));
    }
    Serial.print(F("\t\t"));

    // Dose, Schedule
    Serial.print(F("Hourly Dose: "));
    Serial.print(dose);
    Serial.print(F("ml\t\t"));
    Serial.print(F("Pump Duration: "));
    Serial.print(pump_time[3]*60 + pump_time[4]);
    Serial.println(F("(s)"));
    echo_time();
  }

  // luminance sensor switch
  else if ( command_serial == 'l' ) {
    sense_lux = !sense_lux;
    Serial.print(F("Luminance Sensor: "));
    if ( sense_lux ) {
      Serial.println(F("ON"));
    }
    else {
      Serial.println(F("OFF"));
    }
    echo_time();
  }

  // soil sensor switch
  else if ( command_serial == 's' ) {
    sense_soil = !sense_soil;
    Serial.print(F("Soil Sensor: "));
    if ( sense_soil ) {
      Serial.println(F("ON"));
    }
    else {
      Serial.println(F("OFF"));
    }
    echo_time();
  }

  // water sensor switch
  else if ( command_serial == 'w' ) {
    sense_water = !sense_water;
    Serial.print(F("Water Sensor: "));
    if ( sense_water ) {
      Serial.println(F("ON"));
    }
    else {
      Serial.println(F("OFF"));
    }
    echo_time();
  }

  else if ( command_serial == 't' ) {
    Serial.print(F("Sensor timeout count: "));
    Serial.println(timeout_count);
    echo_time();
  }

  else if ( command_serial == 'm' ) {
    sensor();
    Serial.println(F("Manual Sensor Measurement"));
    echo_time();
  }

  else if ( command_serial == 'p' ) {
    publish_sensor();
    Serial.println(F("Manual Sensor Publishing"));
    echo_time();
  }

  else if ( command_serial == 'd' ) {
    Serial.println(F("Set Nutirent Solution Dose Per Hour"));
    Serial.println(F("(from 0 to 200 in ml): "));
    int temporary_dose = Serial.parseInt();
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
}