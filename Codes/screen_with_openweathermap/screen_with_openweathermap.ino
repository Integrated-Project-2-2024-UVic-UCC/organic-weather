//__________________________________FLOW WORKING STRUCTURE OF SETUP AND LOOP
/*

SETUP
  -> INIT DHT
  -> INIT SCREEN
    -> DISPLAY LOGO welcome(); (initWiFi inside welcome)

LOOP
  -> Try to connect wifi, if isn't it
  -> DATA ACQUIRE (TEMP, OPENWEATHER)
  -> Displays the data screen
  -> waits

*/
//____________________________________LIBRARIES
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "DHT.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 22
#define TFT_CS 27
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 16
#define TFT_MISO 19

#define dht_pin 13

//__________________________________OBJECTS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
DHT dht(dht_pin, DHT22);

//__________________________________WiFi PARAMETERS
// Replace with your network credentials
const char* ssid = "";
const char* password = "";

//__________________________________VARIABLES
float lat = 41.9301200, lon = 2.2548600;

int value, w;
float temperature, humidity, main_temp, temp_max, temp_min;
const char* city;


void initWiFi(){//__________________FOR INIALIZE THE WIFI MODULE
  WiFi.mode(WIFI_STA); //set the esp to a wifi station
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    if(w >=5){
      w=0;
      break;
    }
    w++;
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void initLCD(){
  tft.begin();
  tft.setRotation(3);
  yield();
  tft.fillScreen(ILI9341_WHITE);
  yield();
  welcome();
  delay(400);
}

void data_acquire(){
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (WiFi.status() == WL_CONNECTED) {
    String payload = getOpenWeatherData();
    Serial.print(payload);
    visualizeJson(payload);
  }
  //photodiode data
}

void setup() {
  Serial.begin(9600);

  dht.begin();
  initLCD();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    initWiFi();
  }
  data_acquire();
  data_screen();
  delay(20000);
}

void visualizeJson(String input) { //input payload, the answer of the openweather
  StaticJsonDocument<1024> doc; //reserve 1024 bytes

  //doc stores the json, doc is the root
  DeserializationError error = deserializeJson(doc, input);

  if (error){ //if it can not deserialize the 'input'
    Serial.print("deserializeJson() failsed: ");
    Serial.println(error.c_str());
    return;
  } 

  const char* city_in = doc["name"]; // "Vic"
  city = city_in;

  JsonObject main = doc["main"];
  JsonObject weather = doc["weather"];

  float main_temp_in = main["temp"];
  main_temp = main_temp_in;

  float temp_min_in = main["temp_min"];
  temp_min = temp_min_in;

  float temp_max_in = main["temp_max"];
  temp_max = temp_max_in;

  //esto falla, revisar estructura del arbol
  //char description_in = weather["description"];


}

String getOpenWeatherData(){ //the function that returns a String that do the API call.
  String result = "";
  HTTPClient http; 
  http.begin( //
    "https://api.openweathermap.org/data/2.5/weather" \
    "?lat=41.9301200&lon=2.2548600" \ 
    "&units=metric&lang=en" \
    "&appid=");

    int httpCode = http.GET();
    if (httpCode <=0) {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }else{
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK){
        result = http.getString();
      }
    }
    http.end(); //ends the comunication of http
    return result;
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
  delay(1300);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2.5);
  tft.setCursor(40, 190);
  tft.println("Connecting to WiFi...");
  initWiFi();
  if(WiFi.status() == WL_CONNECTED){
    tft.setCursor(40,210);
    tft.println(WiFi.localIP());
  }else{
    tft.setCursor(40,210);
    tft.println("Failed, trying later");
  }
  yield();
  return micros() - start;
}

unsigned long data_screen() {
  unsigned long start;
  yield();
  tft.fillScreen(ILI9341_WHITE);
  yield();
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1);

  //_______________________________________________linia separadora
  for(int y = 0; y<=239; y++){
    tft.setCursor(320/2,y);
    tft.println("I");
  }
  yield();

  //________________________________________________INSIDE
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(3);
  tft.setCursor(35+15,10);
  tft.println("HOME");

  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(3.7);
  tft.setCursor(30,220/2);
  tft.println(temperature);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(4);
  tft.setCursor(30+105,210/2);
  tft.println("C");

  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
  tft.setCursor(30,70);
  tft.println(humidity);
  tft.setCursor(30+65,70);
  tft.print("%");


  if(WiFi.status() == WL_CONNECTED){
    //________________________________________________CITY
    tft.setTextColor(ILI9341_GREEN); tft.setTextSize(3);
    tft.setCursor(160+35+20,10);
    tft.println(city);

    //_________________________________________________________main temp
    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(3.7);
    tft.setCursor(160+10,220/2);
    tft.println(main_temp);
    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(4);
    tft.setCursor(160+10+105,210/2);
    tft.println("C");
    yield();

    //_________________________________________________________min and max temp
    tft.setTextColor(ILI9341_RED); tft.setTextSize(2.5);
    tft.setCursor(160+50,220/2-20);
    tft.println(temp_max);
    yield();

    tft.setTextColor(ILI9341_BLUE); tft.setTextSize(2.5);
    tft.setCursor(160+50,220/2+30);
    tft.println(temp_min);
    yield();
  }else{
    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(3.7);
    tft.setCursor(160+15,220/2);
    tft.println("NO DATA");
  }
  
  return micros() - start;
} 