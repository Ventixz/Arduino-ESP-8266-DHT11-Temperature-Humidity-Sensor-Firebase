# Arduino-ESP-8266-DHT11-Temperature-Humidity-Sensor-Firebase
# ESP8266 DHT11 to Firebase Data Logger 🌡️💧

This project logs temperature and humidity data from a DHT11 sensor connected to an ESP8266 to a Firebase Realtime Database. The data is sent at different intervals: current, 15 minutes, 30 minutes, 1 day, 3 days, 7 days, 1 month, and 3 months.

## Features 🚀
- Connects ESP8266 to Wi-Fi.
- Reads data from DHT11 sensor.
- Sends current temperature and humidity to Firebase.
- Stores historical data at specified intervals in Firebase.

## Hardware Requirements 🛠️
- ESP8266 board
- DHT11 sensor
- Breadboard and jumper wires

## Software Requirements 💻
- Arduino IDE
- Firebase ESP Client library
- Time library
- NTP Client library

## Installation 🛠️
1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/esp8266-dht11-firebase-logger.git
    cd esp8266-dht11-firebase-logger
    ```

2. **Install required libraries:**
    - [Firebase ESP Client](https://github.com/mobizt/Firebase-ESP-Client)
    - [Time](https://github.com/PaulStoffregen/Time)
    - [NTP Client](https://github.com/arduino-libraries/NTPClient)

3. **Open the project in Arduino IDE.**

## Configuration ⚙️
1. **Set your Wi-Fi credentials:**
    ```cpp
    #define WIFI_SSID "your-ssid"
    #define WIFI_PASSWORD "your-password"
    ```

2. **Set your Firebase project credentials:**
    ```cpp
    #define API_KEY "your-api-key"
    #define USER_EMAIL "your-email"
    #define USER_PASSWORD "your-password"
    #define DATABASE_URL "your-database-url"
    ```

3. **Set the DHT sensor pin:**
    ```cpp
    #define DHTPIN 4 // GPIO pin
    ```

## Code 📜

```cpp
