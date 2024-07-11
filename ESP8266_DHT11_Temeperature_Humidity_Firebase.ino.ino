#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <TimeLib.h>
#include <WiFiUdp.h> 
#include <NTPClient.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

/* 2. Define the API Key */
#define API_KEY ""

/* 3. Define the authentication user Email and password that already registered or added in your project from the firebase*/
#define USER_EMAIL ""
#define USER_PASSWORD ""

/* 4. Define the RealTimeDataBase URL from firebase */
#define DATABASE_URL ""

#define DHTPIN 4          // Pin where the DHT11 is connected
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long currentMillis = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // Update every 60 seconds

void setup()
{
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    config.database_url = DATABASE_URL;

    Firebase.reconnectNetwork(true);

    fbdo.setBSSLBufferSize(4096 , 1024);

    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    Firebase.setDoubleDigits(5);

    config.timeout.serverResponse = 10 * 1000;

    dht.begin();

    timeClient.begin();
    timeClient.update();
    setTime(timeClient.getEpochTime());
}

void loop()
{
    timeClient.update();

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    currentMillis = millis();
    if (Firebase.ready() && (currentMillis - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = currentMillis;

        if (Firebase.RTDB.setFloat(&fbdo, "/dht11/current/humidity", humidity))
        {
            Serial.print("Humidity: ");
            Serial.println(humidity);
        }
        else
        {
            Serial.println("FAILED to send humidity: " + fbdo.errorReason());
        }

        if (Firebase.RTDB.setFloat(&fbdo, "/dht11/current/temperature", temperature))
        {
            Serial.print("Temperature: ");
            Serial.println(temperature);
        }
        else
        {
            Serial.println("FAILED to send temperature: " + fbdo.errorReason());
        }

        storeHistoricalData(humidity, temperature);
    }

    delay(1000);
}

void storeHistoricalData(float humidity, float temperature)
{
    // Save data at different intervals
    saveDataInterval("/dht11/15min", 15 * 60 * 1000, humidity, temperature);
    saveDataInterval("/dht11/30min", 30 * 60 * 1000, humidity, temperature);
    saveDataInterval("/dht11/1day", 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataInterval("/dht11/3days", 3 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataInterval("/dht11/7days", 7 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataInterval("/dht11/1month", 30 * 24 * 60 * 60 * 1000, humidity, temperature);
    saveDataInterval("/dht11/3months", 3 * 30 * 24 * 60 * 60 * 1000, humidity, temperature);
}

void saveDataInterval(String path, unsigned long interval, float humidity, float temperature)
{
    static unsigned long lastSaveMillis15min = 0;
    static unsigned long lastSaveMillis30min = 0;
    static unsigned long lastSaveMillis1day = 0;
    static unsigned long lastSaveMillis3days = 0;
    static unsigned long lastSaveMillis7days = 0;
    static unsigned long lastSaveMillis1month = 0;
    static unsigned long lastSaveMillis3months = 0;

    unsigned long *lastSaveMillis;

    if (path == "/dht11/15min")
    {
        lastSaveMillis = &lastSaveMillis15min;
    }
    else if (path == "/dht11/30min")
    {
        lastSaveMillis = &lastSaveMillis30min;
    }
    else if (path == "/dht11/1day")
    {
        lastSaveMillis = &lastSaveMillis1day;
    }
    else if (path == "/dht11/3days")
    {
        lastSaveMillis = &lastSaveMillis3days;
    }
    else if (path == "/dht11/7days")
    {
        lastSaveMillis = &lastSaveMillis7days;
    }
    else if (path == "/dht11/1month")
    {
        lastSaveMillis = &lastSaveMillis1month;
    }
    else if (path == "/dht11/3months")
    {
        lastSaveMillis = &lastSaveMillis3months;
    }

    if (millis() - *lastSaveMillis >= interval || *lastSaveMillis == 0)
    {
        *lastSaveMillis = millis();

        if (Firebase.RTDB.setFloat(&fbdo, path + "/humidity", humidity))
        {
            Serial.print("Saved humidity to ");
            Serial.println(path);
        }
        else
        {
            Serial.println("FAILED to save humidity: " + fbdo.errorReason());
        }

        if (Firebase.RTDB.setFloat(&fbdo, path + "/temperature", temperature))
        {
            Serial.print("Saved temperature to ");
            Serial.println(path);
        }
        else
        {
            Serial.println("FAILED to save temperature: " + fbdo.errorReason());
        }
    }
}
