

# IoT Low Cost Weather Station 

An implementation IoT low-cost infraestructure, from sensor deploy to visualization.

## Server & Stack
The server is located in a Raspberry Pi 4B  4GB RAM model, running a headless Raspbian, for fast and reliable storage of data, the Raspberry had attached a 120 GB SSD.
![architecture](img/architecture.png)

The RPi hosts use the IOTstack service, concretely the following containers: 

* Node-RED: For easy connection between hardware and software platforms.
* InfluxDB : Storing data.
* MQTT server : Light weigth IOT protocol. 
* Grafana : Simple yet customaizable visualization.


![visualization](img/meteoIOT.png)

## Sensors 

## Display 

## Useful links
* IoTStack : https://sensorsiot.github.io/IOTstack/
* Headless RPi & Networking: https://www.raspberrypi.org/documentation/configuration/wireless/headless.md

