void Sensor()
{ 
  // luminance (i2c: SDA, SCL pin)
  if (senseLux) {
    Serial.println(F("Luminance"));
    float lux = lightMeter.readLightLevel();
    Serial.print(F("Luminance(lx): "));
    Serial.println(lux);
    lux = 0;
  }

  // soil (Serial1 >> modbus : rx1, tx1)
  if (senseSoil) {
  modbus.readHoldingRegisters(soilSensorID, 0, soil_sensor_values, 8);
  Serial.println(F("Soil"));
    float soilHumi = soil_sensor_values[0] / 10.0;
    Serial.print(F("Humi: "));
    Serial.print(soilHumi);
    Serial.print(F("%"));

    float soilTemp = soil_sensor_values[1] / 10.0;
    Serial.print(F("\t\tTemp: "));
    Serial.print(soilTemp);
    Serial.print(F("°C"));

    float soilEC = soil_sensor_values[2] / 100.0;
    Serial.print(F("\t\tEC(dS/m): "));
    Serial.print(soilEC);

    float soilPH = soil_sensor_values[3] / 10.0;
    Serial.print(F("\t\tpH: "));
    Serial.println(soilPH);

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
      soil_sensor_values[i] = 0;
    }
  }

  // water (Serial2: rx2, tx2)
  if (senseWater){
  // nutrient solution (slave 5)
    char readHolingRegisters[8] = { 0x05, 0x03, 0x00, 0x00, 0x00, 0x04, 0x45, 0x8D };
    char waterTempValues[16] = {0};
    int j = 0;
    for (int i=0; i<8 ; i++) {
      Serial2.print(readHolingRegisters[i]);
    }
    delay(50);
    for (int i=0; i<16 ; i++){
      if (Serial2.available()) {
        char data = Serial2.read();
        waterTempValues[j] = data;
        j++;
      }
      else {
        delay(10);
      }
    }

    water_sensor_values[0] = waterTempValues[4]*16*16 + waterTempValues[5];
    water_sensor_values[1] = waterTempValues[6]*16*16 + waterTempValues[7];
    water_sensor_values[3] = waterTempValues[11] - 4294967040;

    Serial.println(F("Nutrient Solution"));
    float nutriTemp = water_sensor_values[3] / 10.0;
    Serial.print(F("Temp: "));
    Serial.print(nutriTemp);
    Serial.print(F("°C"));

    float nutriEC = water_sensor_values[0] / 1000.0;
    Serial.print(F("\t\tEC(dS/m): "));
    Serial.print(nutriEC);

    float nutriPH = water_sensor_values[1] / 100.0;
    Serial.print(F("\t\tpH: "));
    Serial.println(nutriPH);

    // waste water (slave 4)
    delay(100);
    char readHolingRegisters2[8] = { 0x04, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x5C };
    char waterTempValues2[16] = {0};
    int k = 0;
    for (int i = 0; i<8 ; i++) {
      Serial2.print(readHolingRegisters2[i]);
    }
    delay(50);
    for (int i = 0; i<16 ; i++){
      if (Serial2.available()) {
        char data = Serial2.read();
        waterTempValues2[k] = data;
        k++;
      }
      else {
        delay(10);
      }
    }

    water_sensor_values[0] = waterTempValues2[4]*16*16 + waterTempValues2[5];
    water_sensor_values[1] = waterTempValues2[6]*16*16 + waterTempValues2[7];
    water_sensor_values[3] = waterTempValues2[11] - 4294967040;

    Serial.println(F("Waste Water"));
    float wasteTemp = water_sensor_values[3] / 10.0;
    Serial.print(F("Temp: "));
    Serial.print(wasteTemp);
    Serial.print(F("°C"));

    float wasteEC = water_sensor_values[0] / 1000.0;
    Serial.print(F("\t\tEC(dS/m): "));
    Serial.print(wasteEC);

    float wastePH = water_sensor_values[1] / 100.0;
    Serial.print(F("\t\tpH: "));
    Serial.println(wastePH);

    for (int i=0; i<4; i++){
      water_sensor_values[i] = 0;
    }
  }
}


  // air temperature & humidity (Serial2: rx2, tx2)
  // if (senseAir) {
  //   char readInputRegisters[9] = { 0x02, 0x04, 0x00, 0x01, 0x00, 0x02, 0x20, 0x38 };
  //   char airTempValues[10] = {0};
  //   int j = 0;
  //   for (int i = 0; i<8 ; i++) {
  //     Serial1.print(readInputRegisters[i]);
  //   }
  //   delay(50);
  //   for (int i = 0; i<8 ; i++){
  //     if (Serial1.available()) {
  //       char data = Serial1.read();
  //       airTempValues[j] = data;
  //       j++;
  //     }
  //     else {
  //       delay(10);
  //     }
  //   }

  //   air_sensor_values[0] = airTempValues[3]*16*16 - 4294967040 + airTempValues[4];
  //   air_sensor_values[1] = airTempValues[5]*16*16 - 4294967040 + airTempValues[6];

  //   Serial.println(F("Air"));
  //   float temperature = air_sensor_values[0] / 10.0;
  //   Serial.print(F("Temperature: "));
  //   Serial.print(temperature);
  //   Serial.print(F("°C"));

  //   float humidity = air_sensor_values[1] / 10.0;
  //   Serial.print(F("\t\tHumidity: "));
  //   Serial.print(humidity);
  //   Serial.println(F("%"));

  //   for (int i=0; i<2; i++){
  //     air_sensor_values[i] = 0;
  //   }
  //   delay(100);
  // }


  // * modbus >> serial test code
  // char readHolingRegisters[9] = { 0x05, 0x03, 0x00, 0x00, 0x00, 0x04, 0x45, 0x8D };
  // char waterTempValues[17] = {0};
  // int j = 0;
  // for (int i = 0; i<8 ; i++) {
  //   Serial3.print(readHolingRegisters[i]);
  // }
  // Serial.println(Serial3.available());
  // delay(50);
  // while (Serial3.available() > 0) {
  //   char data = Serial3.read();
  //   waterTempValues[j] = data;
  //   j++;
  // Serial.print(data, HEX);
  // Serial.print(":");
  // Serial.print(data, DEC);
  // Serial.print(" ");
  // if (j > 0) {
  //   for (int i = 0; i<16 ; i++){
  //   Serial.print(waterTempValues[i], HEX);
  //   Serial.print(" ");
  //   }
  // }