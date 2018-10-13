# HABControl
Prototype Board &amp; Control Software for High-Altitude-Ballooning. Inspired by the Zephyrus projects at NASA-JPL

**HOW IT WORKS**

HABControl has two "branches" which are under development -- Hardware & Software Modules


**HARDWARE**

HABControl's board is designed to replace the standard Radiosonde setup as well as provide an incredibly easy and intuitive setup for Makers to implement their own projects & experiments. The HABControl board has two parts

*1)* The first part of this board is a standard Radiosonde. It will record data from a suite of sensors including GPS, Humidity Sensor, Therometer, Barometer, Magnetometer, and Accelerometer. This data will be live-streamed to a ground station via radio and stored onto an on-board SD Card for later recovery.

*2)* The second part of this board is the Maker's platform. It will be an empty, unused Arduino (Or similiar Microcontroller) clone with a Serial connection to the Radiosonde in order to pull data from the sensors. This will allow users to create their own experiments and utilize sensor data without tinkering with the "standard" data-collecting portion.

**SAMPLE HABCONTROL CONFIG.TXT**

```
# Callsign, this will essentially be your payload ID
CallSign=ABC123

# Radio frequency, this is the approximate frequency the MTX transmitter will transmit at
# Default is 434.650. Please use the format 434.XYZ
Frequency=434.650

# Sea-Level Pressure for the Barometer, in units hPa. Used for calibration in determining Altitude
# Default is 1013.25
Sea Pressure=1013.25

# Do we want to log data?
# Default is true
Log Data=true

# Name of the data dump file
# Default is data.csv
Data File=data.csv

# Do we want to transmit data over radio?
# Default is true
# NOTE - If you set this to FALSE, please make sure you have AT LEAST one other method of tracking your balloon.
# NOTE - In the United States, you are required to have an Amateur Radio License to transmit on this radio band.
Enable Radio=true
```


 
**SOFTWARE**

HABControl's software will pair with an SDR module to receive, decode & interpret the livestreamed data. Features will additionally include displaying user & balloon location on a map, and potentially predict balloon burst/landing points based off of provided data.

Future iterations of this Software/Board plan to allow for 2-way communication between the ground and payload, potentially allowing for commands to be sent or experiments to be edited in-flight.



**CONTACT**

For any questions or comments regarding HABControl, you can contact me at:

Rohan Daruwala
rdaruwala@gmail.com
