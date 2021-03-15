# Intro (In progress)
Tested on Nextion Basic Display NX4827T043 https://nextion.tech/datasheets/nx4827t043/

Nextion library & config: https://github.com/itead/ITEADLIB_Arduino_Nextion

![nextionDebugg](https://github.com/VicenteYago/OW2Nextion/blob/main/NextionIDE/captions/debuggnextion.PNG)

# Config

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
# Credentials

The following constants are located in `Arduino/libraries/myConfig/myConfig.h` ;)

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
