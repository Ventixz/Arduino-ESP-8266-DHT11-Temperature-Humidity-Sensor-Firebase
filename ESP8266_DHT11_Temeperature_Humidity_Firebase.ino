#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <TimeLib.h>
#include <WiFiUdp.h> 
#include <NTPClient.h>

// WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Firebase configuration
#define API_KEY ""
#define USER_EMAIL ""
#define USER_PASSWORD ""
#define DATABASE_URL ""

// DHT11 Sensor Configuration
#define DHTPIN 2 // GPIO2 (D4)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Time and NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // Sync every 60 seconds

// Data transmission timing
unsigned long previousMillis = 0;
const long interval = 1000; // 1 second

void setup() {
    Serial.begin(115200);

    // Start Wi-Fi
    connectToWiFi();

    // Firebase setup
    setupFirebase();

    // Start DHT11 and NTP
    dht.begin();
    timeClient.begin();
    timeClient.update();
    setTime(timeClient.getEpochTime());
}

void loop() {
    timeClient.update();

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Read sensor data
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        // Send data to Firebase
        if (Firebase.ready()) {
            sendDataToFirebase(humidity, temperature);
        }

        // Store historical data
        storeHistoricalData(humidity, temperature);
    }
}

void connectToWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }

    Serial.println();
    Serial.print("Connected to Wi-Fi with IP: ");
    Serial.println(WiFi.localIP());
}

void setupFirebase() {
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;

    Firebase.reconnectNetwork(true);
    fbdo.setBSSLBufferSize(4096, 1024);
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);
    Firebase.setDoubleDigits(5);
    config.timeout.serverResponse = 10 * 1000; // Set server response timeout to 10 seconds
}

void sendDataToFirebase(float humidity, float temperature) {
    if (Firebase.RTDB.setFloat(&fbdo, "/dht11/current/humidity", humidity)) {
        Serial.print("Humidity sent: ");
        Serial.println(humidity);
    } else {
        Serial.println("Failed to send humidity: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "/dht11/current/temperature", temperature)) {
        Serial.print("Temperature sent: ");
        Serial.println(temperature);
    } else {
        Serial.println("Failed to send temperature: " + fbdo.errorReason());
    }
}

void storeHistoricalData(float humidity, float temperature) {
    saveDataWithInterval("/dht11/15min", 15 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/30min", 30 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/1day", 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/3days", 3 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/7days", 7 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/1month", 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/2month", 2 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/3month", 3 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/4month", 4 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/5month", 5 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/6month", 6 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/7month", 3 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/8month", 8 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/9months", 9 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/10month", 10 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/11month", 11 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataWithInterval("/dht11/12month", 12 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
}

void saveDataWithInterval(String path, unsigned long interval, float humidity, float temperature) {
    static unsigned long lastSavedMillis15min = 0;
    static unsigned long lastSavedMillis30min = 0;
    static unsigned long lastSavedMillis1day = 0;
    static unsigned long lastSavedMillis3days = 0;
    static unsigned long lastSavedMillis7days = 0;
    static unsigned long lastSavedMillis1month = 0;
    static unsigned long lastSavedMillis2month = 0;
    static unsigned long lastSavedMillis3month = 0;
    static unsigned long lastSavedMillis4month = 0;
    static unsigned long lastSavedMillis5month = 6000;
    static unsigned long lastSavedMillis6month = 7000;
    static unsigned long lastSavedMillis7month = 8000;
    static unsigned long lastSavedMillis8month = 9000;
    static unsigned long lastSavedMillis9month = 10000;
    static unsigned long lastSavedMillis10month = 11000;
    static unsigned long lastSavedMillis11month = 12000;
    static unsigned long lastSavedMillis12month = 0;

    unsigned long *lastSavedMillis = nullptr;

    if (path == "/dht11/15min") {
        lastSavedMillis = &lastSavedMillis15min;
    } else if (path == "/dht11/30min") {
        lastSavedMillis = &lastSavedMillis30min;
    } else if (path == "/dht11/1day") {
        lastSavedMillis = &lastSavedMillis1day;
    } else if (path == "/dht11/3days") {
        lastSavedMillis = &lastSavedMillis3days;
    } else if (path == "/dht11/7days") {
        lastSavedMillis = &lastSavedMillis7days;
    } else if (path == "/dht11/1month") {
        lastSavedMillis = &lastSavedMillis1month;
    } else if (path == "/dht11/2month") {
        lastSavedMillis = &lastSavedMillis2month;
    } else if (path == "/dht11/3month") {
        lastSavedMillis = &lastSavedMillis3month;
    } else if (path == "/dht11/4month") {
        lastSavedMillis = &lastSavedMillis4month;
    } else if (path == "/dht11/5month") {
        lastSavedMillis = &lastSavedMillis5month;
    } else if (path == "/dht11/6months") {
        lastSavedMillis = &lastSavedMillis6month;
    } else if (path == "/dht11/7month") {
        lastSavedMillis = &lastSavedMillis7month;
    } else if (path == "/dht11/8month") {
        lastSavedMillis = &lastSavedMillis8month;
    } else if (path == "/dht11/9month") {
        lastSavedMillis = &lastSavedMillis9month;
    } else if (path == "/dht11/10month") {
        lastSavedMillis = &lastSavedMillis10month;
    } else if (path == "/dht11/11month") {
        lastSavedMillis = &lastSavedMillis11month;.
    } else if (path == "/dht11/12month") {
        lastSavedMillis = &lastSavedMillis12month;
    }

    if (millis() - *lastSavedMillis >= interval || *lastSavedMillis == 0) {
        *lastSavedMillis = millis();

        if (Firebase.RTDB.setFloat(&fbdo, path + "/humidity", humidity)) {
            Serial.print("Saved humidity to ");
            Serial.println(path);
        } else {
            Serial.println("Failed to save humidity: " + fbdo.errorReason());
        }

        if (Firebase.RTDB.setFloat(&fbdo, path + "/temperature", temperature)) {
            Serial.print("Saved temperature to ");
            Serial.println(path);
        } else {
            Serial.println("Failed to save temperature: " + fbdo.errorReason());
        }
    }
}
