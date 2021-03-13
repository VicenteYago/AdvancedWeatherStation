# OW2Nextion
Tested on Nextion Basic Display NX4827T043 https://nextion.tech/datasheets/nx4827t043/

Nextion library: https://github.com/itead/ITEADLIB_Arduino_Nextion

In order to use the library update the following lines in `Arduino/libraries/ITEADLIB_Arduino_Nextion-0.7.0/NexConfig.h`

```{c}
....
//#define DEBUG_SERIAL_ENABLE
...
//#define dbSerial Serial
...
#define nexSerial Serial
```
...


