Smart Garden Monitoring System
Overview

The Smart Garden Monitoring System is an IoT-based project designed to monitor and control various parameters of a garden, including temperature, power consumption, and relay controls for devices like water pumps and heating mats.
Features

    Real-time temperature monitoring using DS18B20 sensors.
    Power consumption tracking for voltage, current, power, and power factor using PZEM004Tv30.
    Remote relay control for water pumps and heating mats.
    Weather forecasting and climate visualization with a user-friendly web interface.

Hardware Requirements

    ESP32 or ESP8266 microcontroller.
    DS18B20 temperature sensors.
    PZEM004Tv30 for power monitoring.
    Relays for device control.
    Pump for water
	Heat mat

Software Requirements

    Arduino IDE for ESP32 or ESP8266.
    Required libraries (AsyncWebServer, OneWire, DallasTemperature, PZEM004Tv30, ArduinoJson, Chart.js, Moment.js).

Installation

    Clone the repository to your local machine:

    bash

    git clone https://github.com/your-username/smart-garden-monitoring.git

    Open the Arduino IDE and load the project sketch.

    Install the necessary libraries using the Library Manager in Arduino IDE.

    Configure your WiFi credentials and other settings in the sketch.
	
	Add your appid to the openweather API request within main.js

    Upload the sketch to your ESP32 or ESP8266.

Usage

    Power on the device and connect it to your WiFi network.

    Open a web browser and navigate to the IP address assigned to the device.

    Explore the web interface to monitor temperature, control relays, and view power-related information.

Web Interface

The web interface provides a user-friendly dashboard with real-time data visualization. It includes:

    Climate tracking with temperature graphs.
    Relay control for water pumps and heating mats.
    Daily power consumption and cost estimates.

Contributing

Feel free to contribute to this project by submitting bug reports, feature requests, or pull requests.
License

This project is licensed under the MIT License - see the LICENSE file for details.
Acknowledgments

    Chart.js for the interactive charts.
    Moment.js for date and time manipulation.