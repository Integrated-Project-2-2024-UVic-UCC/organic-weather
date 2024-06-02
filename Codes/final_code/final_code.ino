//_____________________________________________________________________________LIBRARIES
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "DHT.h"
#include "ESP32Time.h"
#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
//*****************************************************************************END



//_____________________________________________________________________________IMAGES AND LOGOS
#include "logo.h"
#include "sun.h"
#include "suncloud.h"
#include "rain.h"
#include "snow.h"
#include "thunder.h"
#include "mist.h"
#include "cozy.h"

//******************************************************************************END



//_____________________________________________________________________________OTHER VARIABLES
int humidity_city_prev;
float temperature_prev, humidity_prev, main_temp_prev, temp_max_prev, temp_min_prev;
float temperature;
String date_prev; String time_prev;
String sky;

float lat = 41.9301200, lon = 2.2548600; //not used as variable
int humidity_city;
float humidity, main_temp, temp_max, temp_min;
const char*city;

unsigned long previous_millis_step = 0; 
unsigned long previous_millis_screen = 0;
unsigned long interval_step = 500;
unsigned long interval_screen = 30000;

//*****************************************************************************END



//_____________________________________________________________________________OBJECTS
#define dht_pin 13
DHT dht(dht_pin, DHT22);
ESP32Time rtc; 
//*****************************************************************************END



//_____________________________________________________________________________TFT ILI9341
#define TFT_DC 22
#define TFT_CS 27
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 12
#define TFT_MISO 19

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void initLCD(){
  tft.begin();
  tft.setRotation(3);
  yield();
  tft.fillScreen(ILI9341_WHITE);
  yield();
  welcome_screen();
}

void draw_box(int y0, int yf, int x0, int amplitude){
  tft.setTextSize(1);
  for(int y = y0; y<=yf; y++){ //VERTICAL LEFT
    tft.setCursor(x0,y);   tft.println(".");
  }
  for(int y = y0; y<=yf; y++){ //VERTICAL RIGHT
    tft.setCursor(x0+amplitude,y);  tft.println(".");
  }
  for(int x = x0; x<=x0+amplitude; x++){ //up fill
    tft.setCursor(x,y0);  tft.println(".");
  }
  for(int x = x0; x<=x0+amplitude; x++){ //down fill
    tft.setCursor(x,yf);  tft.println(".");
  }
}

void refill_box(float var, int y0, int yf, int x0, int amplitude){
  tft.setTextSize(1);
  for(int y = (100-var)*(yf-y0)/100+y0; y<yf; y++) {
    for(int x = x0+3; x<=x0+amplitude-3; x++){
    tft.setCursor(x,y); tft.println(".");
    }
  }
}

void refill_temp_box(float var, int y0, int yf, int x0, int amplitude){
  tft.setTextSize(1);
  for(int y = map(var,-5,40,195,0)+y0; y<yf; y++) {
    for(int x = x0+3; x<=x0+amplitude-3; x++){
    tft.setCursor(x,y); tft.println(".");
    }
  }
}  

unsigned long welcome_screen() {
  unsigned long start;
  tft.drawRGBBitmap(0,0,
    #if defined(__AVR__) || defined(ESP8266)
        logoBitmap, 
    #else
        (uint16_t *)logoBitmap,
    #endif
        LOGO_WIDTH, LOGO_HEIGHT);
  initWiFi();
  return micros() - start;
}

unsigned long background_screen(){
  unsigned long start;
  tft.fillScreen(ILI9341_WHITE);

  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(3); tft.setCursor(35+15,10); tft.println("HOME");//____________HOME TITLE

  if(WiFi.status() == WL_CONNECTED){
    tft.setTextColor(ILI9341_GREEN); tft.setTextSize(3); tft.setCursor(160+35+20,10); tft.println(city);//________CITY TITLE
  } 
  
  tft.setTextColor(ILI9341_BLUE);
  draw_box(35, 230, 2, 8); //___________________HUMIDITY INSIDE BOX
  draw_box(35, 230, 320-8-2-6, 8); //___________HUMIDITY CITY BOX

  tft.setTextColor(ILI9341_BLACK);
  draw_box(35, 230, 320/2-10-2-15, 8); //_______TEMP LEVEL INSIDE
  draw_box(35, 230, 320/2+2+15, 8); //___________TEMP LEVEL CITY

  tft.setTextColor(ILI9341_BLACK);
  draw_box(35, 35, 320/2-10-2-15+2+8+3, 3);                                    draw_box(35, 35, 320/2+2+15-4-3, 3);                                   //40
  draw_box(((230-35)*40/45+35), ((230-35)*5/45+35), 320/2-10-2-15+2+8+3, 3);   draw_box(((230-35)*40/45+35), ((230-35)*5/45+35), 320/2+2+15-4-3, 3);  //35
  draw_box(((230-35)*40/45+35), ((230-35)*10/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*10/45+35), 320/2+2+15-4-3, 3); //30 
  draw_box(((230-35)*40/45+35), ((230-35)*15/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*15/45+35), 320/2+2+15-4-3, 3); //25  
  draw_box(((230-35)*40/45+35), ((230-35)*20/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*20/45+35), 320/2+2+15-4-3, 3); //20  
  draw_box(((230-35)*40/45+35), ((230-35)*25/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*25/45+35), 320/2+2+15-4-3, 3); //15  
  draw_box(((230-35)*40/45+35), ((230-35)*30/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*30/45+35), 320/2+2+15-4-3, 3); //10  
  draw_box(((230-35)*40/45+35), ((230-35)*35/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*35/45+35), 320/2+2+15-4-3, 3); //5
  draw_box(((230-35)*40/45+35), ((230-35)*40/45+35), 320/2-10-2-15+2+8+3, 3);  draw_box(((230-35)*40/45+35), ((230-35)*40/45+35), 320/2+2+15-4-3, 3); //0
  draw_box(230-1, 230-1, 320/2-10-2-15+2+8+3, 3);                              draw_box(230-1, 230-1, 320/2+2+15-4-3, 3);                             //-5
  
  tft.setTextColor(ILI9341_RED);   tft.setTextSize(1.9); tft.setCursor(320/2-3,35+2); tft.println(40);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2-3,((230-35)*5/45+35)+2); tft.println(35);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2-3,((230-35)*10/45+35)+2); tft.println(30);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2-3,((230-35)*15/45+35)+2); tft.println(25);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2-3,((230-35)*20/45+35)+2); tft.println(20);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2-3,((230-35)*25/45+35)+2); tft.println(15);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2-3,((230-35)*30/45+35)+2); tft.println(10);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2,((230-35)*35/45+35)+2); tft.println(5);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1.9); tft.setCursor(320/2,((230-35)*40/45+35)+2); tft.println(0);
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(1.9); tft.setCursor(320/2-4,230-1+2); tft.println(-5);

  tft.drawRGBBitmap(2+8+8,240/2+5,(uint16_t *)cozyBitmap, IM_WIDTH, IM_HEIGHT);

  return micros() - start;
}

unsigned long data_screen() {
  unsigned long start;
  int xtemp=95; int ytemp=210/2-30;

  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2); tft.setCursor(320/2-25, 5); tft.println(date_prev); // CLEAN DATE
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2); tft.setCursor(320/2-25, 5); tft.println(rtc.getTime("%d/%m")); //DATE
  
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2); tft.setCursor(320/2-25, 20); tft.println(time_prev); //CLEAN TIME
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2); tft.setCursor(320/2-25, 20); tft.println(rtc.getTime("%H:%M")); //TIME

  //________________________________________________________________TEMPERATURE
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(3.7);  tft.setCursor(15,ytemp);          tft.println(temperature_prev);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(3.7);  tft.setCursor(15,ytemp);          tft.println(temperature);
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(4);    tft.setCursor(15+xtemp,ytemp-3);  tft.println("C");

  //________________________________________________________________TEMPERATURE LEVEL
  tft.setTextColor(ILI9341_WHITE); refill_temp_box(temperature_prev, 35,  230, 320/2-10-2-15, 8);
  tft.setTextColor(ILI9341_BLACK); refill_temp_box(temperature, 35,  230, 320/2-10-2-15, 8);

  //________________________________________________________________HUMIDITY INSIDE LEVEL
  tft.setTextColor(ILI9341_WHITE);  refill_box(humidity_prev, 35, 230, 2, 8); //CLEAN REFILL
  tft.setTextColor(ILI9341_BLUE);   refill_box(humidity, 35, 230, 2, 8);//REFILL 


  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_____CITY
  if(WiFi.status() == WL_CONNECTED){
    //tft.setTextColor(ILI9341_WHITE); tft.setTextSize(3.7);
    //tft.setCursor(160+50,220/2);  tft.println("NO");  tft.setCursor(160+50,220/2+30);  tft.println("DATA");

    tft.setTextColor(ILI9341_GREEN); tft.setTextSize(3); tft.setCursor(160+35+20,10); tft.println(city);//________CITY TITLE
    
    tft.setTextColor(ILI9341_WHITE);  refill_box(humidity_city_prev, 35, 230, 320-8-2-6, 8); //_____CLEAN HUMIDITY CITY
    tft.setTextColor(ILI9341_BLUE);   refill_box(humidity_city, 35, 230, 320-8-2-6, 8); //__________REFILL WITH HUMIDITY CITY


    //_____________________________________________________________MAIN TEMPERATURE
    tft.setTextColor(ILI9341_WHITE); tft.setTextSize(3.7); tft.setCursor(160+30,ytemp); tft.println(main_temp_prev);
    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(3.7); tft.setCursor(160+30,ytemp); tft.println(main_temp);
    
    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(4);  tft.setCursor(160+30+xtemp,ytemp-3);  tft.println("C");

    //________________________________________________________________TEMPERATURE CITY LEVEL
    tft.setTextColor(ILI9341_WHITE); refill_temp_box(main_temp_prev, 35,  230, 320/2+2+15, 8);
    tft.setTextColor(ILI9341_BLACK); refill_temp_box(main_temp, 35,  230, 320/2+2+15, 8);

    //_____________________________________________________________MAX AND MIN TEMPERATURE
    tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2.5);  tft.setCursor(160+60,ytemp-20);  tft.println(temp_max_prev);
    tft.setTextColor(ILI9341_RED); tft.setTextSize(2.5);  tft.setCursor(160+60,ytemp-20);  tft.println(temp_max);

    tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2.5); tft.setCursor(160+60,ytemp+30);  tft.println(temp_min_prev);
    tft.setTextColor(ILI9341_BLUE); tft.setTextSize(2.5); tft.setCursor(160+60,ytemp+30);  tft.println(temp_min);

    //___________________________PICTURE SELECTION
    if(sky == "broken clouds" or sky == "scattered clouds" or sky == "few clouds" or sky == "overcast clouds"){
      tft.drawRGBBitmap(320/2+2+15+8+10,240/2+5,(uint16_t *)suncloudBitmap, IM_WIDTH, IM_HEIGHT);
    }else if(sky == "clear sky"){
      tft.drawRGBBitmap(320/2+2+15+8+10,240/2+5,(uint16_t *)sunBitmap, IM_WIDTH, IM_HEIGHT);
    }else if(sky == "shower rain" or sky == "rain"){
      tft.drawRGBBitmap(320/2+2+15+8+10,240/2+5,(uint16_t *)rainBitmap, IM_WIDTH, IM_HEIGHT);
    }else if(sky == "thunderstorm"){
      tft.drawRGBBitmap(320/2+2+15+8+10,240/2+5,(uint16_t *)thunderBitmap, IM_WIDTH, IM_HEIGHT);
    }else if(sky == "snow"){
      tft.drawRGBBitmap(320/2+2+15+8+10,240/2+5,(uint16_t *)snowBitmap, IM_WIDTH, IM_HEIGHT);
    }else if(sky == "mist"){
      tft.drawRGBBitmap(320/2+2+15+8+10,240/2+5,(uint16_t *)mistBitmap, IM_WIDTH, IM_HEIGHT); 
    }

  }
  return micros() - start;
} 
//*****************************************************************************END 



//_____________________________________________________________________________PHOTODIODE
#define pd_1 34
#define pd_2 32
int luminosity, luminosity_prev = 0, luminosity_current;

void get_luminosity(){
  luminosity_prev = luminosity;
  luminosity_current = ( analogRead(pd_1) + analogRead(pd_2)) / 2;
  luminosity = (luminosity_prev + luminosity_current) / 2;
  Serial.print("Lum: ");  Serial.println(luminosity);
}
//*****************************************************************************END



//_____________________________________________________________________________STEPPER AND DRIVER
#define IN1  17
#define IN2  16
#define IN3  4
#define IN4  26

float current_step = 0.0; //tells the current position in steps referenced at the 0º at the start
float steps;              //steps needed to achieve the rev_pos, could be negative
int t = 0;
int period = 25;          // ms for ajust the speed

int step_eq [5][4] = { //for more torque needed, could be used when lifting weight
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1},
  {0, 0, 0, 0}
};

void position(float rev_pos){
  //calculate the steps needed to do, in reference to the actual position
  steps = 0;
  steps = (rev_pos/1000.0) * 2048.0 - current_step;
  steps = round(steps);

  Serial.print("Steps needed: ");  Serial.println(steps);

  if(steps>0){ //___________________________________counter clock wise - positive___________________________________
    t = 0;  
    
    while(t < steps){
      for (int i = 0; i < 4; i++){
        digitalWrite(IN1, step_eq[i][0]);
        digitalWrite(IN2, step_eq[i][1]);
        digitalWrite(IN3, step_eq[i][2]);
        digitalWrite(IN4, step_eq[i][3]);
        delay(period);
        //Serial.println(t);
        t++;
      }
    }    
  }


  if(steps<0){ //______________________________________clock wise - negative______________________________________
    t=0;
    while(t > steps){ //steps is a negative value
      for (int i = 3; i>=0; i--){
        digitalWrite(IN1, step_eq[i][0]);
        digitalWrite(IN2, step_eq[i][1]);
        digitalWrite(IN3, step_eq[i][2]);
        digitalWrite(IN4, step_eq[i][3]);
        delay(period);
        //Serial.println(t);
        t--;
      }
    }

  }
  current_step+=t;
  t=0;
  Serial.print("Finish -> ");  Serial.print("Current step: ");  Serial.println(current_step);

  //turn of all the bobines
  digitalWrite(IN1, step_eq[4][0]);
  digitalWrite(IN2, step_eq[4][1]);
  digitalWrite(IN3, step_eq[4][2]);
  digitalWrite(IN4, step_eq[4][3]); 
}
//*****************************************************************************END



//_____________________________________________________________________________WiFi 
const char* ssid = "organic";
const char* password = "organicweather";
void initWiFi(){
  int w = 0;
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
  w=0;
  Serial.println(WiFi.localIP());
}
//*****************************************************************************END 



//_____________________________________________________________________________OPEN WEATHER MAP
void visualizeJson(String input) { //input payload, the answer of the openweather
  StaticJsonDocument<1024> doc; //reserve 1024 bytes

  //doc stores the json, doc is the root
  DeserializationError error = deserializeJson(doc, input);

  if (error){ //if it can not deserialize the 'input'
    Serial.print("deserializeJson() failsed: ");
    Serial.println(error.c_str());
    return;
  } 

  const char* city_in = doc["name"];
  city = city_in;

  JsonObject main = doc["main"];
  JsonObject weather = doc["weather"][0];

  float main_temp_in = main["temp"];
  main_temp = main_temp_in;

  float temp_min_in = main["temp_min"];
  temp_min = temp_min_in;

  float temp_max_in = main["temp_max"];
  temp_max = temp_max_in;

  int humidity_in = main["humidity"];
  humidity_city = humidity_in;

  String sky_in = weather["description"];
  sky = sky_in;
}

String getOpenWeatherData(){ //the function that returns a String that do the API call.
  String result = "";
  HTTPClient http; 
  http.begin( //
    "https://api.openweathermap.org/data/2.5/weather" \
    "?lat=41.9301200&lon=2.2548600" \ 
    "&units=metric&lang=en" \
    "&appid=179f9a1a184ba4f7ba08aecf2b281bca");

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
//*****************************************************************************END



//_____________________________________________________________________________LED RGB
int R, G, B;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 25, NEO_GRB + NEO_KHZ800);
void update_RGB(){
  if(temperature>=-5.0 & temperature<0.0){
    R = map(temperature,-5.0,0,255,100);
    G = map(temperature,-5.0,0,20,0);
    B = map(temperature,-5.0,0,147,255);
  }else if(temperature>=0.0 & temperature<10.0){
    R = map(temperature,0.0,10.0,50,0);
    G = map(temperature,0.0,10.0,0,255);
    B = map(temperature,0.0,10.0,255,128);    
  }else if(temperature>=10.0 & temperature<20.0){
    R = map(temperature,10.0,20.0,0,128);
    G = map(temperature,10.0,20.0,255,255);
    B = map(temperature,10.0,20.0,128,0); 
  }else if(temperature>=20.0 & temperature<30.0){
    R = map(temperature,20.0,30.0,128,255);
    G = map(temperature,20.0,30.0,255,128);
    B = map(temperature,20.0,30.0,0,0);    
  }else if(temperature>=30.0 & temperature<=40.0){
    R = map(temperature,30.0,40.0,255,255);
    G = map(temperature,30.0,40.0,128,0);
    B = map(temperature,30.0,40.0,0,0);  
  }else{
    R = 255;
    G = 255;
    B = 255;
  }
}
//*****************************************************************************END



void data_acquire(){
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  update_RGB();
  for(int n = 0; n <= 11; n++){
    pixels.setPixelColor(n,R,G,B);
  }
  pixels.show(); 

  if (WiFi.status() == WL_CONNECTED) {
    String payload = getOpenWeatherData();
    Serial.print(payload);
    visualizeJson(payload);
  }
}

void data_prev_reload(){

  temperature_prev = temperature;
  humidity_prev = humidity;
  main_temp_prev = main_temp;
  temp_max_prev = temp_max;
  temp_min_prev = temp_min;
  humidity_city_prev = humidity_city;
  date_prev = rtc.getTime("%d/%m");
  time_prev = rtc.getTime("%H:%M");
}


void setup() {
  Serial.begin(115200);

  //_________RGB to green for welcome
  pixels.begin();
  for(int n = 0; n <= 11; n++){
    pixels.setPixelColor(n,0,255,0);
  }
  pixels.show();

  initWiFi();

  configTime(2*3600, 0, "pool.ntp.org");
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo);
  }

  dht.begin();
  initLCD();

  //for secure the position of the motor. Starts at closed, 0 light
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);
  position(-145); 
  current_step = 0.0;

  Serial.println(rtc.getTime("%d/%m/%Y %H:%M:%S"));

  //first data sreen
  if(WiFi.status() != WL_CONNECTED){
    initWiFi();
  }
  data_acquire();
  background_screen();
  data_screen();
  data_prev_reload();

  get_luminosity();
  position(map(luminosity, 0, 4095, 0, 145));
}

void loop() {
  unsigned long current_millis = millis();

  if((unsigned long)(current_millis - previous_millis_step) >= interval_step){

    get_luminosity();
    position(map(luminosity,0, 4095, 0, 145));

    previous_millis_step = millis();
  }

  if((unsigned long)(current_millis - previous_millis_screen) >= interval_screen){

    Serial.println(rtc.getTime("%d/%m/%Y %H:%M:%S"));
    if(WiFi.status() != WL_CONNECTED){
      initWiFi();
    }
    data_acquire();
    data_screen();
    data_prev_reload();

    previous_millis_screen = millis();
  }
  delay(500);
}