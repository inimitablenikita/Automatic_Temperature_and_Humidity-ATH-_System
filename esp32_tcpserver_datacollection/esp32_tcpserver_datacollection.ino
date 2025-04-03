#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4        // GPIO pin for DHT11 data
#define DHTTYPE DHT11   // Define DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi credentials (change these)
const char *ssid = "Your_SSID";
const char *password = "Your_PASSWORD";

WiFiServer server(8080);  // TCP server on port 8080

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("ESP32 Server IP: ");
    Serial.println(WiFi.localIP());

    // Start TCP server
    server.begin();
}

void loop() {
    WiFiClient client = server.available();  // Wait for a client to connect

    if (client) {
        Serial.println("Client connected, sending data...");

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (!isnan(temperature) && !isnan(humidity)) {
            String data = "Temperature: " + String(temperature) + " °C | Humidity: " + String(humidity) + " %\n";
            client.print(data);  // Send data once
            Serial.println("Sent: " + data);
        } else {
            client.println("Failed to read from DHT sensor!");
        }

        delay(500);  // Short delay to ensure data is sent
        client.stop();  // Close the connection
        Serial.println("Client disconnected.");
    }
}