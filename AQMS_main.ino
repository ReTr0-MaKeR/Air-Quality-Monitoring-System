#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RXD2 16
#define TXD2 17

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;

#define mq135 34
bool fresh = true;
int MQ135_data;
int MQ135_THRESHOLD_1 = 2000;

#define led 4

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  lcd.init();
  lcd.backlight();
  pinMode(4,OUTPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Air Quality  ");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring Syst.");
  delay(2000);
  lcd.clear();
}

void loop() {
  read_gasData();
  if (readPMSdata(&Serial1)) {
    displayDataOnLCD();
    delay(5000);
    lcd.clear();
  }
}

boolean readPMSdata(Stream *s) {
  if (!s->available()) {
    return false;
  }
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
  if (s->available() < 32) {
    return false;
  }
  uint8_t buffer[32];
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }
  memcpy((void *)&data, (void *)buffer_u16, 30);
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  return true;
}

float calculateDewPoint(uint16_t humidity) {
  float temp = (float)data.pm25_env / 10.0;
  float hum = (float)humidity / 10.0;
  float dewPoint = temp - (14.55 + 0.114 * temp) * (1 - (0.01 * hum)) - pow(((2.5 + 0.007 * temp) * (1 - (0.01 * hum))), 3) - (15.9 + 0.117 * temp) * pow((1 - (0.01 * hum)), 14);
  return dewPoint;
}

void read_gasData() {
  lcd.clear();
  MQ135_data = analogRead(mq135);
  Serial.println(MQ135_data);
  if (MQ135_data < MQ135_THRESHOLD_1) {
    Serial.println("Fresh Air: ");
    fresh = true;
  } else {
    Serial.print("Poor Air: ");
    fresh = false;
  }
}

void displayDataOnLCD() {
  lcd.setCursor(0, 0);
  lcd.print("PM2.5:");
  lcd.print(data.pm25_env);
  lcd.print("ug/m3");
  lcd.setCursor(0, 1);
  lcd.print("PM10 :");
  lcd.print(data.pm10_env);
  lcd.print("ug/m3");
  delay(2500);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PM1.0 S:");
  lcd.print(data.pm10_standard);
  lcd.print("ug/m3");
  lcd.setCursor(0, 1);
  lcd.print("PM2.5 S:");
  lcd.print(data.pm25_standard);
  lcd.print("ug/m3");
  delay(2500);

  lcd.clear();
  float dp = calculateDewPoint(data.pm25_env);
  lcd.setCursor(0, 0);
  lcd.print("Dew Point:");
  lcd.setCursor(4, 1);
  lcd.print(dp);
  lcd.print(" C");
  delay(2500);
  if (fresh==true) {
    lcd.setCursor(0, 0);
    lcd.print("Gas Sensor Data"); 
    lcd.setCursor(0, 1);
    lcd.print("   Fresh Air   "); 
    delay(2500);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Gas Sensor Data"); 
    lcd.setCursor(0, 1);
    lcd.print("  Gas Detected  ");
    digitalWrite(led,HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led,HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led,HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(500);
  }
}
