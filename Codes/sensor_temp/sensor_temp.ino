#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
 

DHT dht(DHTPIN, DHTTYPE);

int LED_BUILTIN = 2;
int valor;
long temperatura;

/*bool marca = 0;

void IRAM_ATTR isr() {
  marca=!marca;
  delay(100);
  Serial.print(marca);
 }
 */
  
void setup() {
Serial.begin(9600);
Serial.println(F("Sensor DHT22 test!"));
pinMode(LED_BUILTIN, OUTPUT);
dht.begin();
}

void loop() {
delay(500); //Delay per donar temps de reacció al sensor
float h = dht.readHumidity();
float t = dht.readTemperature();
valor=map(h,25,100,250,3000);

/*if (isnan(h) || isnan(t)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}
*/

Serial.print(F("Humitat: "));
Serial.print(h);
Serial.print(F("% Temperatura: "));
Serial.print(t);
Serial.println(F("°C "));
Serial.println(valor);

digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(valor);                       // wait for a second
digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
delay(valor);                       // wait for a second

}
