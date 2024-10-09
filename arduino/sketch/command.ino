void serialEvent() 
{
  char command = Serial.read();  
  // pump manual on/off
  if ( command == '1' ) {
    modbus.writeSingleCoil( relayID, 0x0000, true );
    Serial.println(F("MANUAL PUMP ON"));
    echo_time();
  }
  else if (command == '2') {
    modbus.writeSingleCoil( relayID, 0x0000, false );
    Serial.println(F("MANUAL PUMP OFF"));
    echo_time();
  }

  // led manual on/off
  if ( command == '3' ) {
    modbus.writeSingleCoil( relayID, 0x0005, true );
    Serial.println(F("MANUAL LED ON"));
    echo_time();
  }
  else if (command == '4') {
    modbus.writeSingleCoil( relayID, 0x0005, false );
    Serial.println(F("MANUAL LED OFF"));
    echo_time();
  }

  // get status report manually
  if ( command == 'r' ) {
    char * state[2] = { "ON", "OFF" };
    // LED state
    Serial.print(F("LED state: "));
    if ( relayState[5] ){
      Serial.print(F("ON"));
    }
    else {
      Serial.print(F("OFF"));
    }
    Serial.print(F("\t\t"));

    // Pump state
    Serial.print(F("Pump state: "));
    if ( relayState[0] ) {
      Serial.println(F("ON"));
      echo_time();
    }
    else {
      Serial.println(F("OFF"));
      echo_time();
    }
  }

  // luminance sensor switch
  if ( command == 'l' ) {
    senseLux = !senseLux;
    Serial.print(F("Luminance Sensor: "));
    if ( senseLux ) {
      Serial.println(F("ON"));
      echo_time();
    }
    else {
      Serial.println(F("OFF"));
      echo_time();
    }
  }

  // soil sensor switch
  if ( command == 's' ) {
    senseSoil = !senseSoil;
    Serial.print(F("Soil Sensor: "));
    if ( senseSoil ) {
      Serial.println(F("ON"));
      echo_time();
    }
    else {
      Serial.println(F("OFF"));
      echo_time();
    }
  }

  // water sensor switch
  if ( command == 'w' ) {
    senseWater = !senseWater;
    Serial.print(F("Water Sensor: "));
    if ( senseWater ) {
      Serial.println(F("ON"));
      echo_time();
    }
    else {
      Serial.println(F("OFF"));
      echo_time();
    }
  }

  // // air sensor switch
  // if ( command == 'a' ) {
  //   senseAir = !senseAir;
  //   Serial.print(F("Air Sensor: "));
  //   if ( senseAir ) {
  //     Serial.println(F("ON"));
  //     echo_time();
  //   }
  //   else {
  //     Serial.println(F("OFF"));
  //     echo_time();
  //   }
  // }
}