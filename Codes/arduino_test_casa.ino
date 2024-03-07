#include <ESP32Servo.h>
//#include<Servo.h>

int lightval;
int lightpin=34;
int tm=100;
int servopin=2;
Servo myservo;
int angle;
void setup() {
  Serial.begin(9600);
  pinMode(lightpin,INPUT);
 myservo.attach(servopin);
 pinMode(servopin,OUTPUT);
}

void loop() {
  lightval=analogRead(lightpin);
  Serial.println(lightval);
  delay(tm);
  
  angle= lightval/5;
  myservo.write(angle);
  Serial.println("anlge is");
  Serial.println(angle);

}
