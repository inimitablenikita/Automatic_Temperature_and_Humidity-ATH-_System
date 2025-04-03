import requests
from bs4 import BeautifulSoup
import csv
import time
from datetime import datetime

# URL of the AccuWeather page
url = "https://www.accuweather.com/en/in/gandhinagar/188134/current-weather/188134?unit=c"

# Headers to avoid getting blocked
headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
}

# CSV File Name
csv_filename = "weather_data.csv"

# Function to fetch weather data
def fetch_weather():
    try:
        # Fetch the page
        response = requests.get(url, headers=headers)
        soup = BeautifulSoup(response.text, "html.parser")

        # Extract Temperature
        temp_element = soup.find("div", class_="display-temp")
        temperature = temp_element.get_text(strip=True) if temp_element else "N/A"

        # Extract Humidity & Dew Point
        humidity, dew_point = "N/A", "N/A"
        details = soup.find_all("div", class_="detail-item spaced-content")

        for detail in details:
            label = detail.find("div").text.strip()
            value = detail.find_all("div")[1].text.strip()

            if label.lower() == "humidity":
                humidity = value
            elif label.lower() == "dew point":
                dew_point = value

        # Get current date & time
        current_date = datetime.now().strftime("%Y-%m-%d")
        current_time = datetime.now().strftime("%H:%M:%S")

        return [current_date, current_time, temperature, humidity, dew_point]
    
    except Exception as e:
        print(f"Error fetching weather data: {e}")
        return None

# Function to store data in CSV
def store_data(data):
    # Check if the file exists
    file_exists = False
    try:
        with open(csv_filename, "r") as file:
            file_exists = True
    except FileNotFoundError:
        pass

    # Write data to CSV
    with open(csv_filename, "a", newline="") as file:
        writer = csv.writer(file)

        # Write headers if the file is new
        if not file_exists:
            writer.writerow(["Date", "Time", "Temperature", "Humidity", "Dew Point"])
        
        # Append new row
        writer.writerow(data)

# Main loop to run every hourcle
if __name__ == "__main__":
    while True:
        weather_data = fetch_weather()
        if weather_data:
            store_data(weather_data)
            print(f"Data saved: {weather_data}")
        
        print("Waiting for next hour...\n")
        time.sleep(3600)  # Wait for 1 hour (3600 seconds)