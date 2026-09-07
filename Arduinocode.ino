```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Machine Monitor"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- Wi-Fi ----------
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// ---------- Pins ----------
#define DHT_PIN       4
#define VIBRATION_PIN 18
#define ACS_PIN       35
#define BUZZER_PIN    19

#define SDA_PIN       21
#define SCL_PIN       22

// ---------- Sensors ----------
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_MPU6050 mpu;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ---------- ACS712 ----------
const float ACS_ZERO = 1444.78;   // Calibrated zero-current ADC
const float ACS_SENSITIVITY = 0.100; // 20A module ≈ 100 mV/A

// ---------- Blynk virtual pins ----------
// V0 = Temperature
// V1 = Humidity
// V2 = Current
// V3 = Vibration
// V4 = System Status

BlynkTimer timer;

// --------------------------------------------------
// Read sensors and update system
// --------------------------------------------------

void sendSensorData()
{
  // ---------- DHT11 ----------
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("DHT11 read error!");
    return;
  }

  // ---------- MPU6050 ----------
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x / 9.80665;
  float ay = a.acceleration.y / 9.80665;
  float az = a.acceleration.z / 9.80665;

  float accelerationMagnitude =
      sqrt(ax * ax + ay * ay + az * az);

  // ---------- Vibration ----------
  int vibration = digitalRead(VIBRATION_PIN);

  // ---------- ACS712 ----------
  int adcValue = analogRead(ACS_PIN);

  float current =
      ((float)adcValue - ACS_ZERO) *
      (3.3 / 4095.0) /
      ACS_SENSITIVITY;

  // Remove very small noise
  if (abs(current) < 0.10)
    current = 0.0;

  // Prevent negative display values
  if (current < 0)
    current = 0.0;

  // ---------- Fault detection ----------
  bool fault = false;

  // Example thresholds
  if (temperature > 70.0)
    fault = true;

  if (current > 5.0)
    fault = true;

  if (vibration == HIGH)
    fault = true;

  // ---------- System status ----------
  String status;

  if (fault)
  {
    status = "FAULT";
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    status = "NORMAL";
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ---------- Serial Monitor ----------
  Serial.println();
  Serial.println("---------- SMART MACHINE MONITOR ----------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Acceleration X: ");
  Serial.print(ax);
  Serial.print(" Y: ");
  Serial.print(ay);
  Serial.print(" Z: ");
  Serial.println(az);

  Serial.print("Motion Magnitude: ");
  Serial.print(accelerationMagnitude);
  Serial.println(" g");

  Serial.print("Vibration: ");
  Serial.println(vibration ? "FAULT" : "NORMAL");

  Serial.print("ACS712 ADC: ");
  Serial.println(adcValue);

  Serial.print("Motor Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("System Status: ");
  Serial.println(status);

  Serial.print("Buzzer: ");
  Serial.println(fault ? "ON" : "OFF");

  // ---------- OLED ----------
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("SMART MACHINE MONITOR");

  display.setCursor(0, 12);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 22);
  display.print("Hum : ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 32);
  display.print("Current: ");
  display.print(current, 2);
  display.println(" A");

  display.setCursor(0, 42);
  display.print("Vib: ");
  display.println(vibration ? "FAULT" : "NORMAL");

  display.setCursor(0, 52);
  display.print("Status: ");
  display.println(status);

  display.display();

  // ---------- Blynk ----------
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, current);
  Blynk.virtualWrite(V3, vibration ? 1 : 0);
  Blynk.virtualWrite(V4, status);
}

// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  pinMode(VIBRATION_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  // ---------- I2C ----------
  Wire.begin(SDA_PIN, SCL_PIN);

  // ---------- DHT ----------
  dht.begin();

  // ---------- MPU6050 ----------
  if (!mpu.begin(0x68))
  {
    Serial.println("MPU6050 not detected!");
    while (1)
      delay(100);
  }

  Serial.println("MPU6050 connected.");

  // ---------- OLED ----------
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
  {
    Serial.println("OLED not detected!");
    while (1)
      delay(100);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Smart Machine");
  display.setCursor(20, 40);
  display.println("Monitor");

  display.display();

  delay(2000);

  // ---------- Wi-Fi + Blynk ----------
  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  timer.setInterval(2000L, sendSensorData);
}

// --------------------------------------------------
// Main loop
// --------------------------------------------------

void loop()
{
  Blynk.run();
  timer.run();
}
```

