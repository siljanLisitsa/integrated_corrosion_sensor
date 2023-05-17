# Integrated Corrosion Sensor
Final project spring 2023 (Project II) for Computer Engineering Technology and Computing Science at Algonquin College

## Notes

### Sources
As stated in the code notes in Integrated_Corrosion_Sensor.ino, the following sources were referenced in the making of this sketch:
[Arduino tutorial Simple Web Server Wifi](https://github.com/arduino-libraries/WiFiNINA/blob/master/examples/SimpleWebServerWiFi/SimpleWebServerWiFi.ino), [Arduino tutorial SHT31 Test](https://github.com/adafruit/Adafruit_SHT31/blob/master/examples/SHT31test/SHT31test.ino), [OHMMETER USING ARDUINO by PrinceT1](https://www.instructables.com/OHMMETER-USING-ARDUINO/) and [How To Write Arduino Sensor Data to a CSV File on a Computer by G. Lambert](https://www.circuitbasics.com/logging-arduino-data-to-files-on-a-computer/). 

### Header file for network

The program requires the user to implement a header file with their network IP address and password into the  arduinosecrets.h header file in order to post the sensor data to the webpage. If you would like to run this code, don't forget to put in these strings into the empty arduinosecrets.h file includedin this repository, or make your own.

### Programs

This sensor is run using the Arduino IDE, [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/) (for storing sensor data), and any browser (used to display live sensor readings). 

---

## The project

The purpose of the project was to work from a given design to build a sensor to detect corrosion of a material using factors of time, (ambient) humidity, temperature, and resistance (of the material, in ohms). Below are the Arduino and sensors used in this project:

- Arduino Uno Wifi Rev 2
- Adafruit SHT31-D Temperature & Humidity Sensor
- Ohmmeter (made from scratch)

### The code

The program is divisible into sections by its separate behaviours which were written to meet client requirements as the design evolved and changed. The program was written to display live sensor data to a webpage, and log sensor data to implement a corrosion index. 

### Displaying live sensor data

Lines 156 - 204: The program displays live data for Temperature (°C), Humidity (%), and Resistance (Ω) to the client's browser, and refreshes these values every five seconds. 

### Logging sensor data

Lines 132 - 153: The program converts and stores time (ms), temperature (°C), humidity (%), and resistance (Ω) to a char array (four data points to one line, delimited by a comma). This data is loggable in PuTTY and convertable to a CSV file, from which the data can be read or manipulated as one wishes. 

---

## Extra notes and example

Upon running the program for a test (with no ohmmeter hooked up), the data in a PuTTY terminal looks like this:

![image](https://github.com/siljanLisitsa/integrated_corrosion_sensor/assets/101199129/c1a4732c-8a4a-4e81-b958-71b144167ff7)

From here the data is loggable, and saved to a CSV file (delimited by commas) for creation of a corrosion index. I made a plot of the data in R in order to visualise the test data for review by my peers on the project. 

![Rplot](https://github.com/siljanLisitsa/integrated_corrosion_sensor/assets/101199129/c7cb3cbd-96cf-41d8-b97b-c1d4a0d37138)



