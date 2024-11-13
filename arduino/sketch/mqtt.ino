void ethernet_setup()
{
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
}

void reconnect() 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    Serial.println(mqtt_server);
    
    String clientID = "DS05" + String(random(10000000, 99999999));
  
    if (client.connect(clientID.c_str())) {
      Serial.print(F("Connected to "));
      Serial.println(mqtt_server);
      echo_time();
      message += String("system restart");
      publish_actuator(message);
      client.setKeepAlive(61);
    }
    else {
      Serial.print(F("FAILED, ERROR CODE: "));
      Serial.print(client.state());
      Serial.println(F(" TRYING AGAIN IN 2 SECONDS"));
      delay(2000);
    }
  }
  echo_time();
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  for(int i=0; i<length; i++) {
      Serial.print((char)payload[i]);
  }
  Serial.println();
}

void publish_actuator(String &message) 
{
  if (client.loop() == true) {
    message.toCharArray(publish_payload, message.length()+1);
    client.publish(from_farm_a, publish_payload);
    Serial.print("Publish message: ");
    Serial.println(publish_payload);
  }
}

void publish_sensor() 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  if (client.loop() == true) {
    if (lumi_updated != -1) {
      message += String(lumi_updated)+',';
      message += String(soil_updated[0])+','+String(soil_updated[1])+','+String(soil_updated[2])+',';
      message += String(nutri_updated[0])+','+String(nutri_updated[1])+','+String(nutri_updated[2])+',';
      message += String(waste_updated[0])+','+String(waste_updated[1])+','+String(waste_updated[2])+',';
      message += String(relay_state[0])+','+String(relay_state[5])+','+String(dose);
      message.toCharArray(publish_payload, message.length()+1);
      client.publish(from_farm_s, publish_payload);
      Serial.print("Publish message: ");
      Serial.println(publish_payload);
    }
  }
}


// (현재는 일정 시간마다 노드레드에서 publish 요청을 하므로 스케줄된 publish는 사용하지 않습니다.)
void scheduled_publish(int hour, int min, int sec) 
{
  String message = String(now.year()-2000)+'-'+String(now.month())+'-'+String(now.day())+'-'+String(now.hour())+'-'+String(now.minute())+',';
  bool mqtt_flag_now = false;
  if ( mqtt_time[0] <= min && min < mqtt_time[0] + 1 ) {
    mqtt_flag_now = true;
  }
  else if ( mqtt_time[1] <= min && min < mqtt_time[1] + 1 ) {
    mqtt_flag_now = true;
  }
  else if ( mqtt_time[2] <= min && min < mqtt_time[2] + 1 ) {
    mqtt_flag_now = true;
  }
  else mqtt_flag = false;

  if (!mqtt_flag && mqtt_flag_now){
    if (lumi_updated != -1){
      publish_sensor();
      mqtt_flag = true;
    }
  }
}