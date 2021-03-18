#define _DEBUG_

#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include "EasyNextionLibrary.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <myConfig.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "time.h"

// Nextion display constants
#define ICON1D 1
#define ICON2D 2
#define ICON3D 3
#define ICON4D 4
#define ICON9D 5
#define ICON10D 6
#define ICON11D 7
#define ICON13D 8
#define ICON50D 9

#define ICON1N 10
#define ICON2N 11
#define ICON10N 12

#define STR_BUFF_SIZE 7

EasyNex myNex(Serial1);

// Web client
HTTPClient HTTPclient;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

#include "EasyNextionLibrary.h" 


// OpenWeatherMap Data
const char* hostOpenWeatherMap = "http://api.openweathermap.org/data/2.5/weather";
const char* cityID = "2509402"; 



// Nextion Display Fields
// NexText(PageID, ComponentID, ComponentName)
/*
NexText nexTemp = NexText(0, 7, "t5");
NexText nexHumidity = NexText(0, 3, "t1");
NexText nexWind = NexText(0, 4, "t2");
NexText nexTempMin = NexText(0, 5, "t3");
NexText nexTempMax = NexText(0, 6, "t4");
NexText nexCity = NexText(0, 2, "t0");
NexCrop nexIcon = NexCrop(0, 13, "q0");

NexText nexTempInt = NexText(0, 11, "t8");
NexText nexTempExt = NexText(0, 12, "t9");
//NexText nexNameTempInt = NexText(0, 9, "t6");
//NexText nexNameTempExt = NexText(0, 13, "t7");

NexText nexDay = NexText(0, 10, "t10");
NexText nexTime = NexText(0, 14, "t11");
*/


// timer
unsigned long lastQuery = 0;
unsigned long queryTime = 5000; // Time in miliseconds

// Define NTP Client to get time
const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);



//MQTT
const char* topicInt = "esp8266_A/bme280/values";
const char* topicExt = "esp8266_B/bme280/values";


char tempExt[STR_BUFF_SIZE];
char tempInt[STR_BUFF_SIZE];
char humExt[STR_BUFF_SIZE];
char humInt[STR_BUFF_SIZE];

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

void run() {
  char url[200];
  strcpy(url, hostOpenWeatherMap);
  strcat(url, "?id=");
  strcat(url, cityID);
  strcat(url, "&appid="); 
  strcat(url, OpenWeatherAPIKEY); 

#ifdef _DEBUG_
  Serial.print("HTTP request: ");
  Serial.println(url);
#endif

  // Server conexion & request config
  HTTPclient.begin(url);

  // Send request
  int HTTPcode = HTTPclient.GET();

#ifdef _DEBUG_
  Serial.print("HTTP token: ");
  Serial.println(HTTPcode);
#endif

  if (HTTPcode > 0) {
    if (HTTPcode == HTTP_CODE_OK) {
#ifdef _DEBUG_
      Serial.print("JSON file: ");
      Serial.println(HTTPclient.getString());
#endif

      // Visit https://arduinojson.org/v6/assistant/
      const size_t capacity = JSON_ARRAY_SIZE(3) +
                              2 * JSON_OBJECT_SIZE(1) +
                              JSON_OBJECT_SIZE(2) +
                              3 * JSON_OBJECT_SIZE(4) +
                              JSON_OBJECT_SIZE(5) +
                              JSON_OBJECT_SIZE(6) +
                              JSON_OBJECT_SIZE(12) +
                              304;
      DynamicJsonDocument doc(capacity);

      // JSON Object Parsing
      DeserializationError error = deserializeJson(doc, HTTPclient.getString());
      if (error) {
#ifdef _DEBUG_
        Serial.print("Error parsing JSON. Error: ");
        Serial.println(error.c_str());
#endif
        return;
      }

      // Temperature
      float tempF = doc["main"]["temp"];
      tempF = tempF - 273.15; // to Celsius
      char temperature[STR_BUFF_SIZE];
      snprintf(temperature, STR_BUFF_SIZE, "%.0f C", tempF);

      // humidity
      char humidity[7];
      snprintf(humidity, STR_BUFF_SIZE, "%d%%", (int)doc["main"]["humidity"]) ;


      // Temperature min
      float tempMinF = doc["main"]["temp_min"];
      tempMinF = tempMinF - 273.15; // to Celsius
      char tempMin[7];
      snprintf(tempMin, STR_BUFF_SIZE, "%.0f C", tempMinF);

      // Temperature max
      float tempMaxF = doc["main"]["temp_max"];
      tempMaxF = tempMaxF - 273.15; // to Celsius
      char tempMax[7];
      snprintf(tempMax, STR_BUFF_SIZE, "%.0f C", tempMaxF);

      // windF
      char wind[7];
      snprintf(wind, STR_BUFF_SIZE, "%.0fm/s", (float)doc["wind"]["speed"]);

      // City
      const char* city = doc["name"];
 //     nexCity.setText(city);

      char localt[STR_BUFF_SIZE];
      const char* day = daysOfTheWeek[timeClient.getDay()];
      Serial.println(timeClient.getHours());
      Serial.println(timeClient.getMinutes());

      sprintf(localt,"%d:%d", timeClient.getHours(), timeClient.getMinutes());

      // Update display

     myNex.writeStr("t5.txt", "ON"); 
 /*     
      nexTemp.setText(temperature);
      nexHumidity.setText(humidity);
      nexTempMin.setText(tempMin);
      nexTempMax.setText(tempMax);
      nexWind.setText(wind);
      nexTempInt.setText(tempInt);
      nexTempExt.setText(tempExt);
      nexDay.setText(day);
      nexTime.setText(localt);
*/
      // weather indicator icon 
      const char* icon = doc["weather"][0]["icon"];

      // Icon  assingment 
      if (strcmp(icon, "01d") == 0){
//        nexIcon.setPic(ICON1D);
      } else if (strcmp(icon, "01n") == 0) {
//        nexIcon.setPic(ICON1N);
      } else if (strcmp(icon, "02d") == 0) {
//        nexIcon.setPic(ICON2D);
      } else if (strcmp(icon, "02n") == 0) {
//        nexIcon.setPic(ICON2N);
      } else if (strcmp(icon, "03d") == 0 || strcmp(icon, "03n") == 0) {
//        nexIcon.setPic(ICON3D);
      } else if (strcmp(icon, "04d") == 0 || strcmp(icon, "04n") == 0) {
//        nexIcon.setPic(ICON4D);
      } else if (strcmp(icon, "09d") == 0 || strcmp(icon, "09n") == 0) {
//       nexIcon.setPic(ICON9D);
      } else if (strcmp(icon, "10d") == 0) {
//        nexIcon.setPic(ICON10D);
      } else if (strcmp(icon, "10n") == 0) {
//       nexIcon.setPic(ICON10N);    
      } else if (strcmp(icon, "11d") == 0 || strcmp(icon, "11n") == 0) {
//        nexIcon.setPic(ICON11D);
      } else if (strcmp(icon, "13d") == 0 || strcmp(icon, "13n") == 0) {
//        nexIcon.setPic(ICON13D);
      } else if (strcmp(icon, "50d") == 0 || strcmp(icon, "50n") == 0) {
//        nexIcon.setPic(ICON50D);
      } else {
//        nexIcon.setPic(ICON1D);
      }

#ifdef _DEBUG_
      Serial.println("OpenWeatherMap Data");
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("humidity: ");
      Serial.println(humidity);
      Serial.print("Temp. Min: ");
      Serial.println(tempMin);
      Serial.print("Temp. Max: ");
      Serial.println(tempMax);
      Serial.print("wind: ");
      Serial.println(wind);
      Serial.print("icon: ");
      Serial.println(icon);
      Serial.print("city: ");
      Serial.println(city);
#endif
    } else {
#ifdef _DEBUG_
      Serial.println("Error receiving request.");
#endif
    }
  }
}

void timer() {
  if (millis() < lastQuery) {
    lastQuery = millis();
  }

  if ((millis() - lastQuery) > queryTime) {
    lastQuery = millis();
    run();
   }
}

void readJSON(byte* payload, char *temp, char *hum) {
  StaticJsonDocument<170> doc; 

  char *payload_str = reinterpret_cast<char*>(payload);
  Serial.print("readJSON payload_str: ");
  Serial.println(payload_str);
  
  DeserializationError error = deserializeJson(doc, payload_str);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  snprintf(temp, STR_BUFF_SIZE , "%d C", (int) doc["data"][0]["temperature"]);
  snprintf(hum, STR_BUFF_SIZE , "%d %", (int) doc["data"][0]["humidity"]);
  //snprintf(pres, STR_BUFF_SIZE , "%d", ret[2]);

  Serial.println(temp);
  Serial.println(hum);
  //Serial.println(pres);

  }

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message recived [");
  Serial.print(topic);
  Serial.println("]");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, topicInt) == 0) {
    memset(tempInt, 0, STR_BUFF_SIZE);
    memset(humInt, 0, STR_BUFF_SIZE);
    readJSON(payload, tempInt, humInt);
    
  } else if (strcmp(topic, topicExt) == 0){
    memset(tempExt, 0, STR_BUFF_SIZE);
    memset(humExt, 0, STR_BUFF_SIZE);
    readJSON(payload, tempExt, humExt);
  }
  
}


void reconnect() {
   // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESP8266Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("Succesfully connected");
      // Subscribe
      mqttClient.subscribe("esp8266_A/bme280/values");
      mqttClient.subscribe("esp8266_B/bme280/values");

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(){
  // Initiate communication with display
  // 9600 Default Baudrate
  myNex.begin(9600); 
 #include "EasyNextionLibrary.h" 
  configWifi();
  timeClient.begin();
  mqttClient.setServer(MQTT_HOST_, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop() {
  myNex.NextionListen();
  timeClient.update();
  timer();
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  timeClient.update();
}
