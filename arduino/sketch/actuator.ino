void Pump(int RTCHour, int RTCMin, int RTCSec) 
{
  // uint8_t relayError = 0;
  bool pumpFlagNow = false;
  if ( pumpTime[0] <= RTCHour && RTCHour <= pumpTime[1] ) {
    if ( pumpTime[2] <= RTCMin && RTCMin < pumpTime[2] + pumpTime[3] ) {
      pumpFlagNow = true;
    }
  }

  PumpCheck(pumpFlagNow);

  // scheduled ON
  if (pumpFlagNow && !pumpFlag)
  {
    delay(20);
    modbus.writeSingleCoil( relayID, 0x0000, true );
    Serial.println(F("Pump Start"));
    pumpFlag = true;
    PumpCheckAfter(true);
  }
  // scheduled OFF
  else if (!pumpFlagNow && pumpFlag)
  {
    delay(20);
    modbus.writeSingleCoil( relayID, 0x0000, false );
    Serial.println(F("Pump Stop"));
    pumpFlag = false;
    PumpCheckAfter(false);
  }
}

// on failured schedule
void PumpCheck(bool pumpFlagNow) {
  delay(20);
  modbus.readCoils( relayID, 0x0000, relayState, 8 );
  bool pumpState = relayState[0];
  if ( pumpFlag - pumpState ) {
    if (pumpState) {
      Serial.println(F("PUMP ERROR: UNEXPECTED ON"));
    }
    else {
      Serial.println(F("PUMP ERROR: UNEXPECTED OFF"));
    }
  }
}

// on failured command
void PumpCheckAfter(bool pumpCommand) {
  delay(20);
  modbus.readCoils( relayID, 0x0000, relayState, 8 );
  bool pumpState = relayState[0];
  if ( pumpCommand - pumpState ) {
    if (pumpState) {
      Serial.println(F("PUMP ERROR: OFF COMMAND FAILED"));
    }
    else {
      Serial.println(F("PUMP ERROR: ON COMMAND FAILED"));
    }
  }
}


void Light(int RTCHour, int RTCMin, int RTCSec)
{
  bool ledFlagNow = false;
  if ( ledTime[0] <= RTCHour && RTCHour < ledTime[1] ) { 
    ledFlagNow = true;
  }

   LEDCheck(ledFlagNow);
  
  // scheduled ON
  if (ledFlagNow && !ledFlag)
  {
    delay(20);
    modbus.writeSingleCoil( relayID, 0x0005, true );
    Serial.println(F("LED Start"));
    ledFlag = true;
  }
  // scheduled OFF
  else if (!ledFlagNow && ledFlag)
  {
    delay(20);
    modbus.writeSingleCoil( relayID, 0x0005, false );
    Serial.println(F("LED Stop"));
    ledFlag = false;
  }
}

// on failured schedule
void LEDCheck(bool ledFlagNow) {
  // delay(20);
  // modbus.readCoils( relayID, 0x0000, relayState, 8 );
  bool ledState = relayState[5];
  if ( ledFlag - ledState ) {
    if (ledState) {
      Serial.println(F("LED ERROR: UNEXPECTED ON"));
    }
    else {
      Serial.println(F("LED ERROR: UNEXPECTED OFF"));
    }
  }
}
