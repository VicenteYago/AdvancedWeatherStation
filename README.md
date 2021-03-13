# OW2Nextion
Tested on Nextion Basic Display NX4827T043 https://nextion.tech/datasheets/nx4827t043/

Nextion library & config: https://github.com/itead/ITEADLIB_Arduino_Nextion

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

Usefull tips :
- When uploading the arduino code to the microcontroller disconnect all the wires (RX,TX), remaining only plugged the microUSB cable.
- Its safer to upload the .HMI files to the display using a USB-to-TTL interface than using the microSD because of the poor quality of the  nextion microSD slot. The uploading process using the microSD requires multiple insertions for each update thus increasing the possibility of damaging the slot. 

- For the following exception `[Errno 13] pemission denied: '/dev/ttyUSB0' does not exist or your board is not connected` run `sudo chmod a+rw /dev/ttyUSB0`
