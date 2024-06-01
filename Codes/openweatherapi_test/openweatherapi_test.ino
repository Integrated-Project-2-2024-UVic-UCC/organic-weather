#include <WiFi.h> //for bring the ESP32 the capablity for connect at internet
#include <HTTPClient.h> //For make petitions of an specifi webpage or API call
#include <ArduinoJson.h> //for parse the api answer

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

const char* city;
float main_temp;
float temp_max;
float temp_min;

void initWiFi(){
  WiFi.mode(WIFI_STA); //set the esp to a wifi station
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  initWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String payload = getOpenWeatherData();
    Serial.print(payload);
    visualizeJson(payload);
  }
  delay(3000000); //delay between each API call
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
  JsonObject weatehr = doc["weather"];

  float main_temp_in = main["temp"];
  main_temp = main_temp_in;

  float temp_min_in = main["temp_min"];
  temp_min = temp_min_in;

  float temp_max_in = main["temp_max"];
  temp_max = temp_max_in;
}

String getOpenWeatherData(){ //the function that returns a String that do the API call.
  String result = "";
  HTTPClient http; 
  http.begin( //
    "https://api.openweathermap.org/data/2.5/weather" \
    "?lat=41.9301200&lon=2.2548600" \
    "&units=metric&lang=es" \
    "&appid=xxxxxxxxxxxxxxxx");

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