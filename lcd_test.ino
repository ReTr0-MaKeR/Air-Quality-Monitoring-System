#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for 16 chars and 2 lines
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the LCD
  lcd.init();

  // Turn on the backlight
  lcd.backlight();

  // Print a message to the LCD
  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");

  lcd.setCursor(0, 1);
  lcd.print("ESP32 + I2C LCD");
}

void loop() {
  // The loop doesn't need to do anything in this example
}
