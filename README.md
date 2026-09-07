🔧 Smart Machine Monitor

An ESP32-based IoT machine monitoring and fault detection system designed to continuously monitor important machine parameters such as temperature, humidity, vibration, acceleration, and motor current.

The system displays real-time sensor information on an OLED display and sends the data to a Blynk IoT dashboard for remote monitoring. Basic fault conditions are detected and indicated using a buzzer and system-status alerts.

🚀 Features
🌡️ Real-time temperature monitoring
💧 Humidity monitoring
⚡ Motor current monitoring using ACS712
📳 Vibration detection using SW-420
📐 Acceleration and motion monitoring using MPU6050
🖥️ Local display using OLED SSD1306
📱 Remote monitoring using Blynk IoT
🚨 Fault detection and alert system
🔔 Buzzer-based fault indication
📊 Machine/System status monitoring
🌐 Wi-Fi connectivity through ESP32
🧠 System Overview

The ESP32 acts as the main controller and collects data from multiple sensors connected to the machine.

                  ┌───────────────────┐
                  │      ESP32        │
                  │   Main Controller │
                  └─────────┬─────────┘
                            │
          ┌─────────────────┼──────────────────┐
          │                 │                  │
          ▼                 ▼                  ▼
       DHT11             MPU6050            SW-420
   Temperature &       Acceleration       Vibration
      Humidity           / Motion          Detection
          │                 │                  │
          └─────────────────┼──────────────────┘
                            │
                       ┌────▼─────┐
                       │  ACS712  │
                       │  Current │
                       │  Sensor  │
                       └────┬─────┘
                            │
                     ┌──────▼──────┐
                     │    ESP32    │
                     └──────┬──────┘
                            │
               ┌────────────┴────────────┐
               │                         │
               ▼                         ▼
        OLED SSD1306                Blynk IoT
       Local Monitoring          Remote Monitoring
               │
               ▼
             Buzzer
        Fault Indication
🔧 Hardware Components
Component	Purpose
ESP32 DevKit	Main microcontroller and Wi-Fi communication
DHT11	Temperature and humidity measurement
MPU6050	Acceleration and motion measurement
SW-420	Vibration detection
ACS712	Motor current measurement
OLED SSD1306	Local sensor-data display
Buzzer	Fault/alarm indication
Motor	Machine-load demonstration
Power Supply	System power
📌 Pin Configuration
Component	ESP32 Pin
DHT11 Data	GPIO 4
MPU6050 SDA	GPIO 21
MPU6050 SCL	GPIO 22
SW-420	GPIO 18
ACS712 OUT	GPIO 35
Buzzer	GPIO 19
OLED SDA	GPIO 21
OLED SCL	GPIO 22
I2C Devices
MPU6050 → 0x68
OLED    → 0x3C
⚙️ Working Principle
1. Temperature & Humidity

The DHT11 sensor measures the surrounding temperature and humidity.

The ESP32 reads the sensor values and sends them to both the OLED display and Blynk dashboard.

2. Acceleration & Motion

The MPU6050 provides acceleration data along the X, Y and Z axes.

The acceleration information can be used to identify unusual machine movement or mechanical conditions.

3. Vibration Detection

The SW-420 vibration sensor detects mechanical vibration.

When vibration exceeds the sensor's threshold, the ESP32 can identify a vibration/fault condition.

4. Motor Current Monitoring

The ACS712 measures the current flowing through the motor.

The ESP32 reads the sensor's analog output and converts it into an estimated current value.

Current-sensor calibration is important because the ACS712 has an output offset around its zero-current point.

5. Fault Detection

The ESP32 compares sensor readings against predefined conditions.

Depending on the detected condition, the system can generate:

Normal status
Warning condition
Fault condition
Buzzer alert
6. IoT Monitoring

The ESP32 connects to Wi-Fi and communicates with the Blynk IoT platform.

The dashboard allows the user to remotely monitor machine parameters.

📱 Blynk Dashboard

The Blynk dashboard is used to display:

Temperature
Humidity
Motor Current
Vibration
Acceleration
System Status
Fault Code

This allows the machine condition to be monitored remotely without being physically near the machine.

🖥️ OLED Display

The OLED provides local information such as:

Temperature
Humidity
Motor Current
Vibration Status
Acceleration
System Status
Fault Status

This provides immediate feedback even without accessing the Blynk dashboard.

🚨 Fault Detection

Example conditions that can be monitored:

Condition	Possible Status
Normal temperature	NORMAL
Excessive temperature	WARNING/FAULT
Excessive vibration	FAULT
Abnormal acceleration	WARNING/FAULT
Abnormal motor current	WARNING/FAULT
Multiple abnormal parameters	SYSTEM FAULT

The actual threshold values can be adjusted according to the machine and application.

🛠️ Software & Technologies
Programming Language: Embedded C / Arduino C++
Microcontroller: ESP32
IoT Platform: Blynk IoT
IDE: Arduino IDE
Communication: I2C
Connectivity: Wi-Fi
Sensors: DHT11, MPU6050, SW-420, ACS712
Display: OLED SSD1306
📚 Libraries

Typical libraries used in the project include:

WiFi
Blynk
DHT sensor library
Adafruit MPU6050
Adafruit GFX
Adafruit SSD1306
Wire

Install the required libraries through the Arduino IDE Library Manager before compiling the project.

📊 Example Output
---------- SMART MACHINE MONITOR ----------

Temperature : 25.4 °C
Humidity    : 64.9 %
Acceleration:
X : -0.42
Y :  0.89
Z :  0.20

Vibration   : NORMAL
Motor Current : 0.92 A

Current Status : NORMAL
System Status  : NORMAL
Buzzer         : OFF
🔍 Applications

This project can be adapted for:

Industrial machine monitoring
Motor health monitoring
Predictive maintenance
Factory automation
Equipment condition monitoring
IoT-based maintenance systems
Embedded systems projects
Educational industrial IoT applications
🔮 Future Improvements

Possible improvements include:

Add automatic fault logging
Store historical sensor data in a cloud database
Add machine-learning-based anomaly detection
Add GSM/SMS alerts
Add automatic motor shutdown during critical faults
Add current and vibration trend graphs
Add more accurate vibration measurement using an industrial accelerometer
Add predictive maintenance algorithms
Develop a custom mobile/web dashboard
👨‍💻 Author

Dikshit Tenneni

Electronics & Communication Engineering

Interested in:

Embedded Systems
IoT
VLSI
Robotics
Embedded C
ESP32
STM32
Verilog
⭐ Project Goal

The goal of this project is to demonstrate how embedded systems, sensors, IoT communication and basic fault detection can be combined to create a low-cost machine monitoring system.
