// Simple program for reading Sensirion SEN54 data with an ESP8266 (Wemos D1 mini)
// and publish the data to MQTT endpoints.
// Sensor data is read with an offset to not interfere with another µC reading data via i2c (IKEA vindstyrka)
// Bjoern Heller <tec(att)sixtopia.net>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <SensirionI2CSen5x.h>

// WiFi network settings
const char* ssid = "";
const char* password = "";

// MQTT broker settings
const char* mqttServer = "";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
SensirionI2CSen5x sen5x;

unsigned long lastReadTime = 0;
const long readInterval = 1000; // Read sensor data every 1 second
const long timingOffset = 2500; // Offset to stagger the reading



void setupWiFi() {
    Serial.begin(115200);
    Serial.println("\nConnecting to WiFi");
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting to connect to MQTT broker...");
        if (mqttClient.connect("ESP8266Client_vindstyrka", mqttUser, mqttPassword)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    setupWiFi();
    mqttClient.setServer(mqttServer, mqttPort);
    Wire.begin();
    sen5x.begin(Wire);

    uint16_t error = sen5x.deviceReset();
    if (error) {
        Serial.println("Sensor reset has failed");
    }

    error = sen5x.startMeasurement();
    if (error) {
        Serial.println("Failed to start measurement");
    }
    pinMode(D4, OUTPUT); // Initialize D4 as an output pin for the little blue LED
}

void loop() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();

    unsigned long currentMillis = millis();
    if (currentMillis - lastReadTime >= (readInterval + timingOffset)) {
        lastReadTime = currentMillis;
        digitalWrite(D4, LOW); // Turn ON the LED (Note: D4 is active LOW on D1 Mini)


        float massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0, massConcentrationPm10p0;
        float ambientHumidity, ambientTemperature, vocIndex, noxIndex;

        uint16_t error = sen5x.readMeasuredValues(
            massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0, massConcentrationPm10p0,
            ambientHumidity, ambientTemperature, vocIndex, noxIndex);

        if (error) {
            Serial.println("Failed to read sensor values");
        } else {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%.2f", ambientTemperature);
            mqttClient.publish("/vindstyrka/temp", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", ambientHumidity);
            mqttClient.publish("/vindstyrka/humidity", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", massConcentrationPm1p0);
            mqttClient.publish("/vindstyrka/pm1p0", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", massConcentrationPm2p5);
            mqttClient.publish("/vindstyrka/pm2p5", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", massConcentrationPm4p0);
            mqttClient.publish("/vindstyrka/pm4p0", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", massConcentrationPm10p0);
            mqttClient.publish("/vindstyrka/pm10p0", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", vocIndex);
            mqttClient.publish("/vindstyrka/vocIndex", buffer);

            snprintf(buffer, sizeof(buffer), "%.2f", noxIndex);
            mqttClient.publish("/vindstyrka/noxIndex", buffer);
        }
        digitalWrite(D4, HIGH); // Turn OFF the LED
    }
}
