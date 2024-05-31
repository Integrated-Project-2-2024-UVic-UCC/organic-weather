#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
float temperature;
int R, G, B;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 13, NEO_GRB + NEO_KHZ800);

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
  }
}

void setup() {
  pixels.begin();
}

void loop() {
  for(int i = -5; i<=40; i++){
    temperature=temperature+1.0;
    update_RGB();
    for(int n = 0; n <= 3; n++){
      pixels.setPixelColor(n,R,G,B);
    }
    pixels.show();
    delay(150);
  }
}
