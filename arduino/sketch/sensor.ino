void sensor()
{ 
    // Serial.println(F("Arduino"));
    // float boardTemp = myRTC.getTemperature();
    // Serial.print(F("Temperature: "));
    // Serial.println(boardTemp);
    // boardTemp = 0;

  // luminance (i2c: SDA, SCL pin)
  if (sense_lux) {
    Serial.println(F("<Luminance>"));
    float lux = lightMeter.readLightLevel();
    Serial.print(F("Luminance(lx): "));
    Serial.println(lux);
    lumi_updated = lux;
    lux = -1;
  }

  // soil (Serial1 >> modbus : rx1, tx1)
  if (sense_soil) {
  modbus.readHoldingRegisters(soil_sensor_ID, 0, soil_sensor_values, 8);
  Serial.println(F("<Soil>"));
    // float soilHumi = soil_sensor_values[0] / 10.0;
    // soil_updated[3] = soilHumi;
    // Serial.print(F("Humi: "));
    // Serial.print(soilHumi);
    // Serial.print(F("%\t\t"));

    float soilTemp = soil_sensor_values[1] / 10.0;
    soil_updated[2] = soilTemp;
    Serial.print(F("Temp: "));
    Serial.print(soilTemp);
    Serial.print(F("  "));
    // Serial.print(F("°C"));

    float soilEC = soil_sensor_values[2] / 100.0;
    soil_updated[0] = soilEC;
    Serial.print(F("EC: "));
    Serial.print(soilEC);
    Serial.print(F("  "));

    float soilPH = soil_sensor_values[3] / 10.0;
    soil_updated[1] = soilPH;
    Serial.print(F("pH: "));
    Serial.print(soilPH);
    Serial.println(F("  "));

    // int soilN = soil_sensor_values[4];
    // Serial.print(F("N(mg/kg): "));
    // Serial.print(soilN);

    // int soilP = soil_sensor_values[5];
    // Serial.print(F("\t\tP(mg/kg): "));
    // Serial.print(soilP);

    // int soilK = soil_sensor_values[6];
    // Serial.print(F("\t\tK(mg/kg): "));
    // Serial.println(soilK);

    for (int i=0; i<8; i++){
      soil_sensor_values[i] = -1;
    }
  }

  unsigned long millis_sensor = millis()+200;
  // water (Serial2: rx2, tx2)
  if (sense_water){
  // waste water (slave 4)
    char readHolingRegisters[8] = { 0x04, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x5C };
    unsigned int waterTempValues[16] = {0};
    int j = 0;
    for (int i=0; i<8 ; i++) {
      Serial2.print(readHolingRegisters[i]);
    }
    delay(60);
    char readUntilFour[16];
    Serial2.readBytesUntil(4, readUntilFour, 16);
    delay(60);
    int exit_condition = 0;
    while (exit_condition == 0) {
      char x = Serial2.read();
      if (x == 3){
        if (Serial2.available() >= 14){
          for (int i=0; i<14 ; i++){
            unsigned int data = Serial2.read();
            waterTempValues[j] = data;
            j++;
          }
          exit_condition = 1;
        }
        // timeout
        else if (millis() >= millis_sensor) {
          exit_condition = -1;
        }
      }
      // reading error
      else {
        exit_condition = -1;
      }
    }
    if (exit_condition == -1) {
      timeout_count++;
      // Serial.print(F("ERROR/TIMEOUT COUNT: "));
      // Serial.println(timeout_count);
    }
    else if (exit_condition == 1) {
      water_sensor_values[0] = waterTempValues[2]*16*16 + waterTempValues[3];
      water_sensor_values[1] = waterTempValues[4]*16*16 + waterTempValues[5];
      water_sensor_values[3] = (char)waterTempValues[9] - 4294967040;

      Serial.println(F("<Waste Water>"));
      float wasteTemp = water_sensor_values[3] / 10.0;
      waste_updated[2] = wasteTemp;
      Serial.print(F("Temp: "));
      Serial.print(wasteTemp);
      Serial.print(F("  "));

      float wasteEC = water_sensor_values[0] / 1000.0;
      waste_updated[0] = wasteEC;
      Serial.print(F("EC: "));
      Serial.print(wasteEC);
      Serial.print(F("  "));

      float wastePH = water_sensor_values[1] / 100.0;
      waste_updated[1] = wastePH;
      Serial.print(F("pH: "));
      Serial.println(wastePH);
    }

    unsigned long millis_sensor2 = millis()+200;
    // nutrient solution (slave 5)
    char readHolingRegisters2[8] = { 0x05, 0x03, 0x00, 0x00, 0x00, 0x04, 0x45, 0x8D };
    unsigned int waterTempValues2[16] = {0};
    int k = 0;
    for (int i = 0; i<8 ; i++) {
      Serial2.print(readHolingRegisters2[i]);
    }
    delay(60);
    char readUntilFive[16];
    Serial2.readBytesUntil(5, readUntilFive, 16);
    delay(60);
    int exit_condition2 = 0;
    while (exit_condition2 == 0){
      char x = Serial2.read();
      if (x == 3){
        if (Serial2.available() >= 14){
          for (int i=0; i<14 ; i++){
            unsigned int data = Serial2.read();
            waterTempValues2[k] = data;
            k++;
          }
          exit_condition2 = 1;
        }
        // timeout
        else if (millis() >= millis_sensor2) {
          exit_condition2 = -1;
        }
      }
      // reading error
      else {
        exit_condition2 = -1;
      }
    }
    if (exit_condition2 == -1) {
      timeout_count++;
      Serial.print(F("ERROR/TIMEOUT COUNT: "));
      Serial.println(timeout_count);
    }
    else if (exit_condition2 == 1) {
      water_sensor_values[0] = waterTempValues2[2]*16*16 + waterTempValues2[3];
      water_sensor_values[1] = waterTempValues2[4]*16*16 + waterTempValues2[5];
      water_sensor_values[3] = (char)waterTempValues2[9] - 4294967040;

      Serial.println(F("<Nutrient Solution>"));
      float nutriTemp = water_sensor_values[3] / 10.0;
      nutri_updated[2] = nutriTemp;
      Serial.print(F("Temp: "));
      Serial.print(nutriTemp);
      Serial.print(F("  "));

      float nutriEC = water_sensor_values[0] / 1000.0;
      nutri_updated[0] = nutriEC;
      Serial.print(F("EC: "));
      Serial.print(nutriEC);
      Serial.print(F("  "));

      float nutriPH = water_sensor_values[1] / 100.0;
      nutri_updated[1] = nutriPH;
      Serial.print(F("pH: "));
      Serial.println(nutriPH);

    }
    for (int i=0; i<4; i++){
      water_sensor_values[i] = -1;
    }
  }
}

// (현재는 일정 시간마다 노드레드에서 센서 작동을 요청하므로 스케줄된 센서 작동은 사용하지 않습니다.)
void scheduled_sensor(int hour, int min, int sec) 
{ 
  static int sensor_error = 0;
  bool sensor_flag_now = false;
  if ( sensor_time[0] <= min && min < sensor_time[0] + 1 ) {
    sensor_flag_now = true;
  }
  else if ( sensor_time[1] <= min && min < sensor_time[1] + 1 ) {
    sensor_flag_now = true;
  }
  else if ( sensor_time[2] <= min && min < sensor_time[2] + 1 ) {
    sensor_flag_now = true;
  }
  else sensor_flag = false;
  if (!sensor_flag && sensor_flag_now){
    sensor();
    //sensor_check(nutri_updated, waste_updated, sensor_error);
    sensor_flag = true;
  }
}

void sensor_check(float* nutri_updated, float* waste_updated, int sensor_error)
{
  float value = (nutri_updated[2]-25.6)*(waste_updated[2]-25.6);
  for (int i=0; i<3; i++){
    value = value * nutri_updated[i];
  }
  for (int i=0; i<3; i++){
    value = value * waste_updated[i];
  }
  if (sensor_error < 5){
    if (value == 0){
      sensor_error++;
      sensor();
    }
    else sensor_error = 0;
  }
  else {
    Serial.println(F("WATER sensor ERROR COUNT: "));
    Serial.println(timeout_count);
    sense_water = false;
    for (int i=0; i<3; i++){
    nutri_updated[i] = -1;
    waste_updated[i] = -1;
    }
  }
}

  // * modbus >> serial test code
  // char readHolingRegisters[9] = { 0x05, 0x03, 0x00, 0x00, 0x00, 0x04, 0x45, 0x8D };
  // char waterTempValues[17] = {0};
  // int j = 0;
  // for (int i = 0; i<8 ; i++) {
  //   Serial2.print(readHolingRegisters[i]);
  // }
  // Serial3.println(Serial2.available());
  // delay(50);
  // while (Serial2.available() > 0) {
  //   char data = Serial2.read();
  //   waterTempValues[j] = data;
  //   j++;
  // }
  // Serial3.print(data, HEX);
  // Serial3.print(":");
  // Serial3.print(data, DEC);
  // Serial3.print(" ");
  // if (j > 0) {
  //   for (int i = 0; i<16 ; i++){
  //   Serial3.print(waterTempValues[i], HEX);
  //   Serial3.print(" ");
  //   }
  // }
