int MQ135_THRESHOLD_1 = 2000;
void setup()
{
  Serial.begin(115200);
  pinMode(4,OUTPUT);
}

void loop()
{
  int MQ135_data = analogRead(34);
  Serial.println(MQ135_data);
  if (MQ135_data < MQ135_THRESHOLD_1) {
    Serial.println("Fresh Air: ");
    
  } else {
    Serial.print("Poor Air: ");
    digitalWrite(4,HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    delay(500);
  }
  delay(1000);
}