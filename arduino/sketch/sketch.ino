#include "credential.h"   // (#define device_mac *YOUR_MAC_ADDRESS)
#include <Wire.h>                  
#include <BH1750.h>
#include <DS3231.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ModbusRTUMaster.h>    


//// Ethernet Setting (mac address in credential.h)
  byte mac[] = {device_mac};
  int ethernetCSPin = 53;
  EthernetClient ethClient;


//// MQTT settings 
  bool mqtt_flag = false;  // current MQTT action goal: (default: false)

  const char mqtt_server[] = "test.mosquitto.org";
  const char from_farm_a[] = "farmease/actuator";
  const char from_farm_s[] = "farmease/sensor";
  const char to_farm[] = "farmease/command";
  
  void callback(char* topic, byte* payload, unsigned int length);
  PubSubClient client(mqtt_server, 1883, callback, ethClient);

  char publish_payload[100];

  volatile int mqtt_time[3] = { 10, 30, 50 };


//// modbus settings
  ModbusRTUMaster modbus(Serial1);


//// relay settings
  bool relay_state[8];
  bool led_flag = true;       // current LED action goal: (default: true)
  bool pump_flag = false;     // current pump action goal: (default: false)

  uint8_t relay_ID = 1;       // 0x00~0x07 (0:pump, 3: main led, 5:side led)

  volatile int dose = 50;
  volatile int pump_time[5] = { 7, 17, 0, 1, 30 };   //  from 7:00 to 17:00, for 1 minute
  volatile int led_time[3] = { 8, 18, 0 };           //  from 8:00 to 18:00


//// sensor settings
  bool sensor_flag = false; // current sensor action goal: (default: false)

  // sensor (de)activation (true = active)
  bool sense_lux = true;
  bool sense_soil = true;
  bool sense_water = true;
  // bool sense_air = true; (in repair)

  uint8_t soil_sensor_ID = 7;  
  byte luxAddress = 0x23;
  BH1750 lightMeter;  // i2c (sda 20, scl 21)

  // temporary buffers
  uint16_t air_sensor_values[2] = {0};
  uint16_t soil_sensor_values[8] = {0};
  uint16_t water_sensor_values[4] = {0};

  // latest measurements
  float lumi_updated = -1;   // default: -1 (for error checking)
  float soil_updated[4] = {0};
  float nutri_updated[3] = {0};
  float waste_updated[3] = {0};
  
  int timeout_count = 0;
  volatile int sensor_time[3] = { 5, 25, 45 };


//// RTC(DS3231) Setting 
  DS3231 myRTC; 
  DateTime now;
  bool pmFlag = false; 
  bool century = false; 
  bool h12Flag = false;       
  byte rtcAddress = 0x68;                   
  
  // change when setting time manually
  // (RTC 시간을 수동으로 조정합니다.)
  byte rtcTime[6] = { 24, 9, 12, 18, 5, 0 };  // (yy,m,d,h,m,s)
  bool rtcSetTime = false; // true = set time manually (default: false)
  static int hour, min, sec;


//// millis settings
  unsigned long millis_short = 0;
  unsigned long interval_short = 1000;


// =====================================================================
void setup() 
{
  Serial.begin(9600);    // PC
  Serial1.begin(9600);   // RS485 MODBUS RTU (relay, soil sensor)
  Serial2.begin(9600);   // RS485 to TTL (water1, water2)
  Serial3.begin(9600);   // TTL (PC)
  modbus.begin(9600);    // MODBUS
  Wire.begin();          // I2C 
  lightMeter.begin();    // I2C 

  ethernet_setup();
  client.setServer(mqtt_server, 1883);
  // client.subscribe(to_farm);

  setup_rtc(rtcSetTime, rtcTime);

  now = RTClib::now();
  hour = myRTC.getHour(h12Flag, pmFlag);
  min = myRTC.getMinute();
  sec = myRTC.getSecond();

  dose_to_time(dose);
  echo_time();
}


void loop()
{

  // MQTT reconnection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // short-term actions (1s)
  unsigned long current_millis = millis();
  if (current_millis - millis_short >= interval_short) 
  {
    millis_short = current_millis; 

    now = RTClib::now();
    hour = myRTC.getHour(h12Flag, pmFlag);
    min = myRTC.getMinute();
    sec = myRTC.getSecond();

    from_nodered();
    scheduled_pump(hour, min, sec);
    scheduled_led(hour, min, sec);

    // scheduled_sensor(hour, min, sec);
    // scheduled_publish(hour, min, sec);
  }
}
