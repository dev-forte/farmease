/*
2024/10/09
1) 통신 배선 및 코드 수정
Serial  : PC
Serial1 : RS485 to TTL (modbus: 릴레이, 토양 센서)
Serial2 : RS485 to TTL (급액, 배액 센서) (공기 온습도 센서는 센서값 오류, 통신 방해로 제외함)
Serial3 : TTL (PC)

2) 센서 사용/미사용 수동 명령어 추가 (l, s, w)
3) LED 작동 로직 수정, 오작동 확인 기능 추가
4) 토양 센서 NPK값 부정확하여 사용하지 않음

남은 과제:
1) LED 릴레이 배선 (+ 릴레이 slave ID 변경)
2) 팬(타이머 콘센트) 가동
3) 구동계 작동 알고리즘 변경 (flag 및 배액율 기준 작동 시간 변경)
... Node-Red 프로토타입 / MQTT 통신 기능 / 데이터 저장 부분 연동
*/

#include <Wire.h>                  
#include <BH1750.h>
#include <DS3231.h>
#include <Ethernet.h>                
#include <ModbusRTUMaster.h>             
#include <LiquidCrystal_I2C.h>     

  // modbus slave IDs (uint8_t), register address (uint16_t)
  uint8_t relayID = 1;                 // 0x00~0x07 (0:pump, 5:side led)
  uint8_t soilSensorID = 7;            // 0x00~0x08 (0-6 in use)
  // uint8_t waterSensorAID = 4;          // waste water
  // uint8_t waterSensorBID = 5;          // nutrient solution 
  // uint8_t airSensorID = 2;             // 0x01~0x02 (in repair)

  // buffers (array of uint16_t)
  // bool error = false;
  bool relayState[8];
  bool pumpFlag = false;               // current pump state goal: (default: false)
  bool ledFlag = false;                // current LED state goal: (default: false)
  uint16_t air_sensor_values[2];
  uint16_t soil_sensor_values[8];
  uint16_t water_sensor_values[4];

  // I2C slave address (byte)
  byte rtcAddress = 0x68;
  byte luxAddress = 0x23;
  byte lcdAddress = 0x27;

  // Ethernet Setting (mac address: credential)
  int ethernetCSPin = 53;
  byte mac[] = {credential_mac};

  // RTC(DS3231) Setting 
  DS3231 myRTC; 
  DateTime now;
  bool pmFlag = false; 
  bool century = false; 
  bool h12Flag = false;
  bool rtcSetTime = false;                    // true = set time manually (default: false)
  byte rtcTime[6] = { 24, 9, 12, 18, 5, 0 };  // (yy,m,d,h,m,s)

  // objects, timing
  ModbusRTUMaster modbus(Serial1);           // RS485 MODBUS RTU
  // LiquidCrystal_I2C lcd(lcdAddress, 16, 2);  // i2c (sda 20, scl 21)
  BH1750 lightMeter;                         // i2c (sda 20, scl 21)

  unsigned long millisPrevShort = 0
  unsigned long millisPrevLong = 0;
  unsigned long intervalShort = 1000;        // actuator
  unsigned long intervalLong = 3000;         // sensor values, actuator status report 



  // Schedule
  volatile int pumpTime[4] = { 7, 17, 0, 1 }; //  from 7:00 to 17:00, for 1 minute
  volatile int ledTime[3] = { 8, 18, 0 };     //  from 8:00 to 18:00


  // Report Setting (true = report sensor/setting information)
  bool senseLux = true;
  bool senseSoil = true;
  bool senseWater = true;
  // bool senseAir = true; (in repair)
  // bool sensorReport = true;
  // bool statusReport = true;



void setup() {
  Serial.begin(9600);    // PC
  Serial1.begin(9600);   // RS485 MODBUS RTU (relay, soil sensor)
  Serial2.begin(9600);   // RS485 to TTL (water1, water2)
  Serial3.begin(9600);   // TTL (PC2)
  modbus.begin(9600);    // MODBUS
  Wire.begin();          // I2C 
  lightMeter.begin();    // I2C 

  // Ethernet (mandatory) 
  Ethernet.init(ethernetCSPin);  
  while (!Serial) {  ; } 
  Serial.println("Initialize Ethernet with DHCP:"); 
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");}
      else if (Ethernet.linkStatus() == LinkOFF)
       {Serial.println("Ethernet cable is not connected.");}
    while (true) {delay(1);}
  }
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  setup_rtc(rtcSetTime, rtcTime);
  echo_time();
  
  // LCD (no use)
  // lcd.init();
  // lcd.clear();
  // lcd.backlight();
  // lcd.print(F("Hello!"));
}


void loop() {
  // time check
  unsigned long currentMillis = millis();
  now = RTClib::now();
  int RTCHour = myRTC.getHour(h12Flag, pmFlag);
  int RTCMin = myRTC.getMinute();
  int RTCSec = myRTC.getSecond();

  // short-term actions (1s)
  if (currentMillis - millisPrevShort >= intervalShort) {
    millisPrevShort = currentMillis; 

    Pump(RTCHour, RTCMin, RTCSec);
    Light(RTCHour, RTCMin, RTCSec);
    // Sensor();
  }

  // long-term actions (3s)
  if (currentMillis - millisPrevLong >= intervalLong) {
    millisPrevLong = currentMillis; 

    Sensor();
  }

  // halt if error
  // if (error) {
  //   while(1){};
  // }
}
