import socket
import time
import csv
from datetime import datetime

ESP32_IP = "10.200.19.246"  # Replace with your ESP32 server IP
PORT = 80
CSV_FILE = "sensor_data.csv"

# Create or append to the CSV file and write the headers if needed
def initialize_csv():
    try:
        with open(CSV_FILE, mode='a', newline='') as file:
            writer = csv.writer(file)
            # Write header only if the file is empty
            if file.tell() == 0:
                writer.writerow(["Date", "Time", "Temperature (°C)", "Humidity (%)"])
    except Exception as e:
        print(f"Error initializing CSV file: {e}")

def save_to_csv(temperature, humidity):
    try:
        now = datetime.now()
        date_str = now.strftime("%Y-%m-%d")
        time_str = now.strftime("%H:%M:%S")
        
        with open(CSV_FILE, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([date_str, time_str, temperature, humidity])
        
        print(f"Data saved: {date_str} {time_str} | Temp: {temperature}°C | Humidity: {humidity}%")
    except Exception as e:
        print(f"Error saving data to CSV: {e}")

def fetch_sensor_data():
    while True:
        try:
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((ESP32_IP, PORT))
            print(f"Connected to ESP32 at {ESP32_IP}:{PORT}")

            # Receive data once
            data = client_socket.recv(1024).decode().strip()
            if data:
                print(f"Received: {data}")

                # Assuming data format: "Temperature: XX.X °C | Humidity: YY.Y %"
                try:
                    temp_humid = data.split("|")
                    temperature = temp_humid[0].split(":")[1].strip().split(" ")[0]
                    humidity = temp_humid[1].split(":")[1].strip().split(" ")[0]

                    save_to_csv(temperature, humidity)
                except IndexError:
                    print("Invalid data format received")

            client_socket.close()
            print("Connection closed. Waiting 1 hour before reconnecting...")

        except Exception as e:
            print(f"Error: {e}")

        # Wait 1 minute before reconnecting
        time.sleep(3600)

if __name__ == "__main__":
    initialize_csv()
    fetch_sensor_data()