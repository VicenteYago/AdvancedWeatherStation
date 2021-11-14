# IoT Weather Station

Build your own IoT weather station based in ESP8266. 

![nextionDebugg](https://github.com/VicenteYago/AdvancedWeatherStation/blob/main/img/architecture.PNG)

The proyect its composed by 4 parts: 
* IOT sensors for indoors & outdoors
* ESP8266 boards
* Rpi as a server
* Nextion display

We will sense the following variables :

- temperature (ºC)
- relative humidty (%)
- pressure (Pa)

The data will be managed by IOTStack (https://github.com/SensorsIot/IOTstack) 
 services but concretelly i choose the following : 
 
 * Node-RED: For easy connection between hardware devices and software platforms
 * InfluxDB: Time series database
 * Mosquitto: MQTT server
 * Grafana : Analytics and interactive visualization web app




Tested on [Nextion Basic Display NX4827T043](https://nextion.tech/datasheets/nx4827t043/)

# Config

## Arduino Libraries

* AsynqMqttClient : https://github.com/marvinroger/async-mqtt-client
* PubSubClient: https://www.arduino.cc/reference/en/libraries/pubsubclient/
* ArduinoJson : https://github.com/bblanchon/ArduinoJson
* AdafruitBME280: https://github.com/adafruit/Adafruit_BME280_Library
* ESPAsyncTCP : https://github.com/me-no-dev/ESPAsyncTCP
* TimeLib : https://github.com/PaulStoffregen/Time

* Nextion libraries are unmantained and full of bugs, afortunately [Seithan](https://github.com/Seithan/EasyNextionLibrary) provided an excellent one with a very good documentaion. Additionally [Perry Pebbington](https://forum.arduino.cc/index.php?topic=604185.msg4100443#msg4100443) and  [Ray Livingston](https://forum.arduino.cc/index.php?topic=620821.0) also offer solutions to the original nextion libraries. But the Seithan one is the only that fits all my needs.


In order to use the library update the following lines in `Arduino/libraries/ITEADLIB_Arduino_Nextion-0.7.0/NexConfig.h`

```{c}
....
//#define DEBUG_SERIAL_ENABLE
...
//#define dbSerial Serial
...
#define nexSerial Serial
...
```

## Raspberry pi set-up
- https://desertbot.io/blog/headless-raspberry-pi-4-ssh-wifi-setup
- https://sensorsiot.github.io/IOTstack/Getting-Started.html

# Credentials

The following constants are located in `Arduino/libraries/myConfig/myConfig.h`, you must define yours ;).

```{C}
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define MQTT_USER "MQTT_USER"
#define MQTT_PASS "MQTT_PASS"
#define MQTT_HOST_1 192
#define MQTT_HOST_2 168
#define MQTT_HOST_3 12
#define MQTT_HOST_4 103
#define MQTT_HOST_ "192.168.12.103"
#define MQTT_PORT 1883

#define OpenWeatherAPIKEY "APIKEY"
```

# Usefull tips 
- When uploading the `OW2nextion.ino` to the microcontroller disconnect all the nextion display wires (RX,TX), so that only  the microUSB cable remains connected. Once finished reconnect then and reset the board to begin the comunication with the  display. 

- Its safer to upload the .HMI files to the nextion display using a USB-to-TTL interface than using the microSD because of the poor quality of the  nextion microSD slot. The uploading process using the microSD requires multiple insertions for each update thus increasing the possibility of damaging the slot. 

- Save your money and stuck with an basic nextion display unless you really need the bigger sizes (> 7.0"), in that case go for the 'intelligent hmi display' series. Most of the premium 'intelligent display' capabilites  like transparent images can be handled with tricks like cropped images.

- For the following Arduino IDE exception `[Errno 13] pemission denied: '/dev/ttyUSB0' does not exist or your board is not connected` run `sudo chmod a+rw /dev/ttyUSB0`




