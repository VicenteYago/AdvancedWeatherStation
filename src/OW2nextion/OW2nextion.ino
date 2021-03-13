#define _DEBUG_

#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include "Nextion.h"
#include <ArduinoJson.h>
#include <myConfig.h>

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

// Web client
HTTPClient HTTPclient;

// OpenWeatherMap Data
String hostOpenWeatherMap = "http://api.openweathermap.org/data/2.5/weather";
String cityID = "2509402"; 

// Nextion Display Fields
// NexText(PageID, ComponentID, ComponentName)
NexText nexTemp = NexText(0, 7, "t5");
NexText nexHumidity = NexText(0, 3, "t1");
NexText nexWind = NexText(0, 4, "t2");
NexText nexTempMin = NexText(0, 5, "t3");
NexText nexTempMax = NexText(0, 6, "t4");
NexText nexCity = NexText(0, 2, "t0");

NexCrop nexIcon = NexCrop(0, 13, "q0");

NexText nexTempExt1 = NexText(0, 11, "t8");
NexText nexTempExt2 = NexText(0, 12, "t9");
NexText nexNameTempExt1 = NexText(0, 9, "t6");
NexText nexNameTempExt2 = NexText(0, 13, "t7");

// timer
unsigned long lastQuery = 0;
unsigned long queryTime = 5000; // Time in miliseconds

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
  String url = hostOpenWeatherMap;
  url += "?id=";
  url += cityID;
  url += "&appid=";
  url += OpenWeatherAPIKEY;

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
      char temp[7];
      snprintf(temp, 7, "%.0f C", tempF);

      // humidity
      String humidityS = String(int(doc["main"]["humidity"])) + "%";
      char humidity[7];
      humidityS.toCharArray(humidity, 7);

      // Temperature min
      float tempMinF = doc["main"]["temp_min"];
      tempMinF = tempMinF - 273.15; // to Celsius
      char tempMin[7];
      snprintf(tempMin, 7, "%.0f C", tempMinF);

      // Temperature max
      float tempMaxF = doc["main"]["temp_max"];
      tempMaxF = tempMaxF - 273.15; // to Celsius
      char tempMax[7];
      snprintf(tempMax, 7, "%.0f C", tempMaxF);

      // windF
      float windF = doc["wind"]["speed"];
      char wind[8];
      snprintf(wind, 8, "%.0fm/s", windF);

      // City
      const char* city = doc["name"];
      nexCity.setText(city);
      
      // Update display
      nexTemp.setText(temp);
      nexHumidity.setText(humidity);
      nexTempMin.setText(tempMin);
      nexTempMax.setText(tempMax);
      nexWind.setText(wind);

      // weather indicator icon 
      const char* icon = doc["weather"][0]["icon"];

      // Icon  assingment 
      if (strcmp(icon, "01d") == 0){
        nexIcon.setPic(ICON1D);
      } else if (strcmp(icon, "01n") == 0) {
        nexIcon.setPic(ICON1N);
      } else if (strcmp(icon, "02d") == 0) {
        nexIcon.setPic(ICON2D);
      } else if (strcmp(icon, "02n") == 0) {
        nexIcon.setPic(ICON2N);
      } else if (strcmp(icon, "03d") == 0 || strcmp(icon, "03n") == 0) {
        nexIcon.setPic(ICON3D);
      } else if (strcmp(icon, "04d") == 0 || strcmp(icon, "04n") == 0) {
        nexIcon.setPic(ICON4D);
      } else if (strcmp(icon, "09d") == 0 || strcmp(icon, "09n") == 0) {
        nexIcon.setPic(ICON9D);
      } else if (strcmp(icon, "10d") == 0) {
        nexIcon.setPic(ICON10D);
      } else if (strcmp(icon, "10n") == 0) {
        nexIcon.setPic(ICON10N);    
      } else if (strcmp(icon, "11d") == 0 || strcmp(icon, "11n") == 0) {
        nexIcon.setPic(ICON11D);
      } else if (strcmp(icon, "13d") == 0 || strcmp(icon, "13n") == 0) {
        nexIcon.setPic(ICON13D);
      } else if (strcmp(icon, "50d") == 0 || strcmp(icon, "50n") == 0) {
        nexIcon.setPic(ICON50D);
      } else {
        nexIcon.setPic(ICON1D);
      }

#ifdef _DEBUG_
      Serial.println("OpenWeatherMap Data");
      Serial.print("Temperature: ");
      Serial.println(temp);
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

void setup(){
  // Initiate communication with display
  // 9600 Default Baudrate
  nexInit(); // From NextHardware.h
  configWifi();
}

void loop() {
  timer();
}
