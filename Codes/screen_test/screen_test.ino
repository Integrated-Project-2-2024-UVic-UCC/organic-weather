#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "DHT.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 22
#define TFT_CS 27
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 16
#define TFT_MISO 19

#define dht_pin 13

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
DHT dht(dht_pin, DHT22);

int value;
float temperature;
float humidity;

void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 

  dht.begin();
 
  tft.begin();
  tft.setRotation(3);
  yield();
  tft.fillScreen(ILI9341_WHITE);
  yield();

  welcome();
  delay(2000);
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  data_screen();
  delay(1000);
}

unsigned long welcome() {
  unsigned long start;
  tft.fillScreen(ILI9341_WHITE);
  yield();
  tft.setCursor(15, 60);
  yield();
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(7);
  tft.println("ORGANIC");
  yield();
  tft.setCursor(15, 120);
  tft.println("WEATHER");
  yield();
  return micros() - start;
}

unsigned long data_screen() {
  unsigned long start;
  yield();
  tft.fillScreen(ILI9341_WHITE);
  yield();
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
  tft.setCursor(10,10);
  tft.println(temperature);
  yield();
  return micros() - start;
}