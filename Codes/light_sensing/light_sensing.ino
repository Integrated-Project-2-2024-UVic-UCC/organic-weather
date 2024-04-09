int sensor = 12;
int sensor_value;
void setup() {
  Serial.begin(9600);

}

void loop() {
  sensor_value = analogRead(sensor);
  Serial.println(sensor_value);
  delay(1000);
}
