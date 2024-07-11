# Arduino-ESP-8266-DHT11-Temperature-Humidity-Sensor-Firebase
ESP8266 DHT11 Sensor Data to Firebase
This project demonstrates how to use an ESP8266 microcontroller to read temperature and humidity data from a DHT11 sensor and send the data to a Firebase Realtime Database. The data is updated at various intervals: current, 15 minutes, 30 minutes, 1 day, 3 days, 7 days, 1 month, and 3 months.

Features
Reads temperature and humidity data from a DHT11 sensor.
Sends the current temperature and humidity data to Firebase every second.
Stores historical data at intervals of 15 minutes, 30 minutes, 1 day, 3 days, 7 days, 1 month, and 3 months.
Uses NTP (Network Time Protocol) to synchronize time.
Hardware Requirements
ESP8266 microcontroller
DHT11 sensor
Wi-Fi network
Software Requirements
Arduino IDE
Firebase ESP8266 Client Library
DHT Sensor Library
TimeLib Library
NTPClient Library
Installation
Install the required libraries in the Arduino IDE.
Clone this repository.
Open the provided .ino file in the Arduino IDE.
Update the Wi-Fi credentials, Firebase API key, user email and password, and Firebase database URL in the code.
Upload the code to your ESP8266 microcontroller.
