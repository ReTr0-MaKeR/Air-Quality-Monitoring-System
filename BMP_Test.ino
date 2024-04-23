#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"

Adafruit_BMP280 bmp;

float pressure;
float temperature;

void setup() {
  bmp.begin(0x76, 100); // Initialize the sensor with its I2C address
  Serial.begin(115200); // Start serial communication
  Serial.println("BMP280 Test:");
}

void loop() {
  pressure = bmp.readPressure();
  temperature = bmp.readTemperature();

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000); // Update every 1 second
}
