#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <myConfig.h>

#define MQTT_HOST IPAddress(MQTT_HOST_1, MQTT_HOST_2, MQTT_HOST_3, MQTT_HOST_4)

// MQTT Topics
#define TOPIC "esp8266_B/ds18b20/values"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Variables to hold sensor readings
const int oneWireBus = 4;     
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float temp;


void configWifi() {
#ifdef _DEBUG_
  Serial.print("Connecting with ");
  Serial.println(WIFI_SSID);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef _DEBUG_
    Serial.print(".");
#endif
  }

#ifdef _DEBUG_
  Serial.println("");
  Serial.println("Connecting to WiFi");
#endif
}

void reconnect() {
   // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESP8266_B Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("Succesfully connected");
      // Subscribe
      mqttClient.subscribe(TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void dsbtoJson(char *buffer, int epochtime, float temp){//, float hum, float pres){
  StaticJsonDocument<170> doc;
  doc["mqtt_topic"] = TOPIC;
  doc["timestamp"] = epochtime; 
  JsonArray data = doc.createNestedArray("data"); 
  JsonObject data_0 = data.createNestedObject();
  data_0["temperature"] = temp;
  serializeJson(doc, buffer,170);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  configWifi();
  timeClient.begin();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(callback);
  
  timeClient.update();
  unsigned long currentMillis = millis();
  unsigned long epochTime = timeClient.getEpochTime();

  sensors.begin();
  sensors.requestTemperatures(); 
  temp = sensors.getTempCByIndex(0);

  if (!mqttClient.connected()) {
    reconnect();
  }
  
  char buffer[170];
  dsbtoJson(buffer,epochTime, temp);//, hum, pres);
  
  Serial.printf("Publishing on topic %s", TOPIC);
  if (mqttClient.publish(TOPIC, buffer)) {
    Serial.printf("Message: %s \n", buffer);
    Serial.println("Publish ok");
  }
  else {
    Serial.println("Publish failed");
  }
  ESP.deepSleep(60*1000); //10 minutes
}

void loop(){

}
