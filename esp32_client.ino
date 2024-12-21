//-------------------------------------------------------------------------------------------------------------
//PROJECT:      Ground Water Measurement
//Program:      ESP32 Sensor Client
//LV:           Advanced Programming in C++
//Author:       Andreas Schriefl
//Version:      1.0
//Describtion:  This Client should read an hydrostatic sensor and calculate the current water level of ground water.
//              The data shall be transmitted to a server (via MQTT), where it is processed, stored and monitored.
//              When the water reaches a critical level, an alarm mechanism shall be triggered. 
//-------------------------------------------------------------------------------------------------------------
#include <WiFi.h>           //WiFi connection
#include <PubSubClient.h>   //MQTT Module
#include <driver/adc.h>     //for analog reading settings
#include <Preferences.h>    //enables a secure storage mechanism for ESP32 - to store WiFi password

//MQTT information
String* mqtt_user = NULL;                         //MQTT User variable (filled from secure storage)
String* mqtt_pw = NULL;                           //MQTT password variable (filled from secure storage)
Preferences preferences;                          //instantiate a secure storage for credentials
const char* mqtt_server = "192.168.1.225";        //MQTT broker IP
const int mqtt_port = 1883;                       //MQTT broker port
String mqtt_clientName = "ESP32 - Brunnen";       //MQTT client name
WiFiClient espClient;                             //create Wifi client
PubSubClient client(espClient);                   //tell MQTT cleint, wich wifi client to use
long lastMsg = 0;                                 //timestamp of last sensor reading or mqtt message
const long sensorReadInterval = 30000;            //time between sensor readings (messages to mqtt broker) 
//level sensor measurement parameters 
const float resistor = 147.6;                     //value of measurement resistor (Ohm)
const int gpio_pin = 36;                          //reading pin of the sensor
const int readingsPerMeasurement = 1000;          //for one measurement, do multiple readings and calculate average
const float iRangeSensor = 0.016;                 //current range of the sensor (datasheet 4mA-20mA => 16mA)
const float iOffsetSensor = 0.0108;               //no-load-current of the sensor => 0% waterlevel

//-------------------------------------------------------------------------------------------------------------
// INITIALIZE
//-------------------------------------------------------------------------------------------------------------
void setup() {
  delay(200);
  Serial.begin(115200);                       //initialize OPTIONAL Diplay

  //open preferences in read-write mode
  preferences.begin("creds", false);          //secure storage for WiFi/MQTT credentials

  //--------------------------------------------------------
  //uncomment this to save the password once on ESP storage
  //preferences.putString("ssid", "REALssid");
  //preferences.putString("password", "REALwifiPassword");
  //preferences.putString("mqtt-user", "REALmqttUserName");
  //preferences.putString("mqtt-pw", "REALmqttPassword");
  //--------------------------------------------------------

  String ssid = preferences.getString("ssid", "defaultSSID");             //get SSID from secure storage
  String password = preferences.getString("password", "defaultPassword"); //get PW from secure storage

  //debug msg on OPTIONAL Display
  Serial.println("");
  Serial.print("Connecting to: ");
  Serial.print(ssid);

  //initiate Wifi connection
  WiFi.begin(ssid.c_str(), password.c_str()); //try to connect to Network
  //wait until connection is established
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");                        //print loading dots "......"
  }

  Serial.println("");
  Serial.println("WiFi connected, IP: ");
  Serial.print(WiFi.localIP());

  //connect to MQTT server
  client.setServer(mqtt_server, mqtt_port);   //set MQTT borker information
  
  //set analog Read resolution for sensor readings
  analogReadResolution(12);                   //max resolution = 12 bit (4095)
  analogSetAttenuation(ADC_11db);             //adjust input range to full range (from 0 to 3.3V)
}

//-------------------------------------------------------------------------------------------------------------
// MAIN - LOOP
//-------------------------------------------------------------------------------------------------------------
void loop() {
  //as long as MQTT server is not connected, try reconntect
  if (!client.connected()){
    reconnect();
  }

   long currentTime = millis();        //get current timestamp
  //check the time since the last reading and 
  if (currentTime - lastMsg > sensorReadInterval){
    lastMsg = currentTime;
  
    //fetch sensor values
    long rawLevelSensor = readAnalog(gpio_pin);
    float waterlevel = convertLevel(rawLevelSensor);
    String payload = String(waterlevel);
    Serial.println(payload);
    client.publish("/heizung/brunnen/waterlevel", payload.c_str());
    payload = String(rawLevelSensor);
    //send debug information
    Serial.println(payload);
    client.publish("/heizung/brunnen/rawLevel", payload.c_str());
  }
}

//-------------------------------------------------------------------------------------------------------------
//function if mqtt connection is not established
void reconnect() {
  while(!client.connected()){
    Serial.print("MQTT disconnected - trying to connect");
    
    //get MQTT credentials from secure storage
    String mqtt_user = preferences.getString("mqtt-user", "defaultUser");     	  //get MQTT user from secure storage
    String mqtt_pw = preferences.getString("mqtt-pw", "defaultMQTTpw");           //get MQTT pw from secure storage

    //connect to MQTT broker with client name
    if(client.connect("ESP - Brunnen 2", mqtt_user.c_str(), mqtt_pw.c_str())){
      Serial.println("Connected!");
    }
    else {
      Serial.print("MQTT connection failed, code: ");
      Serial.print(client.state());                     //get connection status and print it as error code
      delay(30000);                                     //reconnect if needed after 30s
    }

  }
}

//-------------------------------------------------------------------------------------------------------------
//function to convert the analog value of the sensor to the water level
  float convertLevel(long raw){
    const float vRef = 3.3;             //reference voltage 3.3V = 100%
    const long maxValue = 4095;         //max value the reading can get = 100%
    const float maxWaterLevel = 20;     //max Sensor reading 20m = 100% = 20mA Sensor Current (Sensor Range 4-20mA - where 4mA is 0m)
    //calculate voltage on resitor to get current flow of the sensor
    float vMeasurement = (float)raw/(float)maxValue * vRef;     //%age of reading times max reference voltage
    float iMeasurement = vMeasurement/resistor;                 //I=U/R
    float level = 0;
    //check if calculated current value is in the range of sensor (4-20mA), otherwise ERROR
    //include 5% tolerance for range
    if (iMeasurement <= 0.02*1.05 && iMeasurement >= 0.004*0.95){
      //map the current flow to a water level
      level = (iMeasurement - iOffsetSensor)/(iRangeSensor)*20.0;     //normalize current to zero (4mA = 0m = 0%) and calculate percentage of max current -> multiply by max height
    }
    else {
      level = 0;                                          //invalid value
    }

    //sending debug information
    String payload = String(vMeasurement);
    Serial.println(payload);
    client.publish("/heizung/brunnen/vMeasurement", payload.c_str());
    payload = String(iMeasurement);
    Serial.println(payload);
    client.publish("/heizung/brunnen/iMeasurement", payload.c_str());

    return level;     //returns value in meter
  }

//-------------------------------------------------------------------------------------------------------------
//function for measure X times and calc the average
long readAnalog(int pin){
  long int reading = 0;
  //do multiple readings and calculate average
  for (int i = 0; i < readingsPerMeasurement; i++ ){
    reading += analogRead(gpio_pin);
    
    //debug information - send only one probe
    if (i == 0){
      //sending debug information
      String payload = String(reading);
      Serial.println(payload);
      client.publish("/heizung/brunnen/singeAnalogReading", payload.c_str());
    }
  }
  return (long) (reading/readingsPerMeasurement);     //calculate average and add 0.5 to change from round down to round to nearest
}

