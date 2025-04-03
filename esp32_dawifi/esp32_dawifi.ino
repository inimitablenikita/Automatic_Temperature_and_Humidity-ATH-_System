#include <WiFi.h>
#include <esp_wpa2.h>  // WPA2 Enterprise support
#include <esp_wifi.h>
#include <DHT.h>
#include <DHT_U.h>


#define DHTPIN 4       // GPIO pin for DHT11 data
#define DHTTYPE DHT11  // Define DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "DAIICT_Student";  // WiFi SSID
const char *identity = "202315004";   // Student ID (identity)
const char *password = "Papabunu@02";  // WiFi password

WiFiServer server(80);  // TCP server on port 1234

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)identity, strlen(identity));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)identity, strlen(identity));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
  esp_wifi_sta_wpa2_ent_enable();
  WiFi.begin(ssid);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(1000);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());

    Serial.print("ESP32 MAC Address: ");
    Serial.println(WiFi.macAddress());
    // Serial.println("\nConnected to Wi-Fi!");
    // Serial.print("ESP32 IP: ");
    // Serial.println(WiFi.localIP());
    // Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.subnetMask());


    server.begin();  // Start the TCP server
    Serial.println("TCP Server started.");
    esp_wifi_set_promiscuous(true);

  } else {
    Serial.println("\nFailed to connect. Retrying in 10 seconds...");
    delay(10000);
    connectToWiFi();
  }
}


void setup() {
  Serial.begin(115200);
  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_STA);
  // Serial.println("Connecting to Wi-Fi...");

  // Enable WPA2 Enterprise
  // esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)identity, strlen(identity));
  // esp_wifi_sta_wpa2_ent_set_username((uint8_t *)identity, strlen(identity));
  // esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
  // esp_wifi_sta_wpa2_ent_enable();  // Correct function for WPA2 Enterprise

  // WiFi.begin(ssid);

  // Wait for connection
  // int retries = 0;

  // while (WiFi.status() != WL_CONNECTED&& retries < 20) {
  //   delay(1000);
  //   Serial.print(".");
  //   retries++;
  // }

  // Serial.print("WiFi Status: ");
  // Serial.println(WiFi.status());

  // Serial.print("ESP32 MAC Address: ");
  // Serial.println(WiFi.macAddress());
  // Serial.println("\nConnected to Wi-Fi!");
  // Serial.print("ESP32 IP: ");
  // Serial.println(WiFi.localIP());
  // // Serial.println(WiFi.localIP());
  // Serial.println(WiFi.gatewayIP());
  // Serial.println(WiFi.subnetMask());
  dht.begin();
  connectToWiFi();
  server.begin();

  // server.begin();  // Start the TCP server
  // Serial.println("TCP Server started.");

  // esp_wifi_set_promiscuous(true);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi Disconnected! Reconnecting...");
        connectToWiFi();
    }

  WiFiClient client = server.available();  // Check for incoming client
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

    delay(500);     // Short delay to ensure data is sent
    client.stop();  // Close the connection
    Serial.println("Client disconnected.");
  }

  static unsigned long lastPingTime = 0;
    if (millis() - lastPingTime > 1800000) {  // 30 minutes
        Serial.println("Sending keep-alive ping...");
        WiFi.disconnect();
        connectToWiFi();
        lastPingTime = millis();
  }

}