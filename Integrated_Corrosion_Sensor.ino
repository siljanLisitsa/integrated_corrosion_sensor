/************************************************************
  Integrated Corrosion Sensor
  Spring 2023

  @author Siljan Blondin & Sawyer Austen
  
  Code excerpts were taken from Arduino tutorials [1-2], OHMMETER USING ARDUINO by PrinceT1 [3], and How To Write Arduino Sensor 
  Data to a CSV File on a Computer by G. Lambert [4]
  ([1] https://github.com/arduino-libraries/WiFiNINA/blob/master/examples/SimpleWebServerWiFi/SimpleWebServerWiFi.ino
   [2] https://github.com/adafruit/Adafruit_SHT31/blob/master/examples/SHT31test/SHT31test.ino
   [3] https://www.instructables.com/OHMMETER-USING-ARDUINO/
   [4] https://www.circuitbasics.com/logging-arduino-data-to-files-on-a-computer/)

*************************************************************/

#include <WiFiNINA.h> //Wifi and server library
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h" //Sensor library


//WIFI AND WEB SERVER
//Wifi network pass and ID are declared here and defined in arduino_secrets.h
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);


//SENSORS
//Create Adafruit_SHT31 object to call built-in functions on SHT31 Temp and Humidity Sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

//Ohmmeter variables
int analogPin = 0;    //analog pin: input of voltage across resistor
int raw = 0;          //analog voltage values
int Vin = 5;          //input voltage
float Vout = 0;       //output voltage
float R1 = 10000;     //10k ohm resistor
float R2 = 0;         //test material resistance
float result = 0;     //placeholder for intermediate calculations

//CSV Variables
char dataStr[100] = "";
char buffer[9];


void setup() {
  //Start serial monitor for display of values from sensors
  Serial.begin(9600);

  /* SENSORS TESTING *****************************************************/
  /*Test to confirm sensor is connected and behaving normally
    (returns TRUE if found and normal, FALSE if not)*/
  Serial.print("SHT31 Test...");
  if (sht31.begin(0x44)) {
    Serial.println("SHT31 found OK");
  } else {
    Serial.println("Couldn't find SHT31");
    while (1) delay (1);
  }//end of if-else

  /*Block to check whether heater is or is not enabled. Heater on will
    fluctuate the temperatureerature by ~3*C).*/
  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");


  /*OHMMETER **************************************************************/
  raw = analogRead(analogPin);
  if (raw)
  {
    result = raw * Vin;
    Vout = (result) / 1024.0;
    result = (Vin / Vout) - 1;

    R2 = R1 * result;  //will give resistance value of test material

    delay(1000);
  }//end of if


  /* WIFI/NETWORK TESTING ************************************************/
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }//end of if

  // check that firmware is up to date
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }//end of if

  // attemperaturet to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }//end of while

  server.begin();
  //connected, prints wifi status
  printWifiStatus();
  
  /*CSV HEADERS **********************************************************************/
  //This line is to put column headers over the data for any manipulation later (in excel or R)
  Serial.println("Time, Temperature, Humidity, Resistance");

}//end of setup


void loop() {

  /* READING DATA FROM SENSORS *******************************************************/
  /*Float values 'temperature' and 'humidity' are to read/store temperature and humidity
    from the sht31 sensor, respectively.*/
  float temperature   = sht31.readTemperature();
  float humidity  = sht31.readHumidity();

  /*PUTTING SENSOR DATA INTO CSV FILE FOR EXPORT *************************************/
  //Clears dataStr of any leftover inputs
  dataStr[0] = 0;

  //Converts millis long to char array
  ltoa(millis(), buffer, 10);           //convert long to charStr
  strcat(dataStr, buffer);              //add it to the end
  strcat( dataStr, ", ");               //append the delimiter

  //dtostrf(floatVal, minimum width, float precision, character array);
  dtostrf(temperature, 4, 1, buffer);   //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat( dataStr, buffer);             //add it to the end
  strcat( dataStr, ", ");               //append the delimiter

  dtostrf(humidity, 4, 1, buffer);      //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat( dataStr, buffer);             //append the converted float
  strcat( dataStr, ", ");               //append the delimiter

  dtostrf(R2, 4, 1, buffer);            //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat( dataStr, buffer);             //append the converted float

  Serial.println(dataStr);


  /* SETTING UP SERVER AND CLIENT *****************************************************/
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          //client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          // client.println("<!DOCTYPE HTML>");
          // client.println("<html>");
          /*Will display Temperature, Humidity and Resistance values to webpage and refresh 
           * for current values, every 5 seconds*/
          client.print("Temperature (*C) = "); client.print(temperature); client.print("\t\t");
          client.print("Humidity (%) = "); client.println(humidity);
          client.print("Resistance (Ohms) = "); client.println(R2);
          client.println("</html>");
          break;
        }//end of if c

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }//end of if else
      }//end of if available
    }//end of while

    // give the web browser time to receive the data
    delay(1000);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");

  }//end of if

  delay(1000);

}//end of loop


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}//end of printWifiStatus
