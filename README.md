# 🌍 Data Collection & Verification Using ESP32  

## 📌 Project Overview  
This project utilizes the **ESP32 WiFi module** with **Arduino IDE** to collect real-time environmental data. To ensure accuracy, **Python web scraping** is used to fetch online weather data for verification. A **client-server model** enables data transfer, and a **local web page** displays both ESP32-collected and online data side by side.  

## 🚀 Features  
✅ **ESP32 Data Collection** – Collects real-time sensor data  
✅ **Web Scraping** – Fetches online weather data for verification  
✅ **Client-Server Communication** – ESP32 sends data to a local server  
✅ **Local Web Page** – Displays both sensor and online data for easy comparison  

## 🛠️ Tech Stack  
- **ESP32 WiFi Module**  
- **Arduino IDE**  
- **Python (Web Scraping)**  
- **Flask (Local Server for Web Page)**  
- **HTML, CSS, JavaScript (Web Display)**  

## 🔧 Installation & Setup  
### 1️⃣ **Hardware Setup**  
- Connect the ESP32 module to the necessary sensors.  
- Upload the Arduino code using **Arduino IDE**.  

### 2️⃣ **Python & Web Server Setup**  
- Install required Python libraries:  
  ```bash
  pip install requests beautifulsoup4 flask
