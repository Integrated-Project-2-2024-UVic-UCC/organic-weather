/*
pos negativa, el eje baja, las hojas se cierran

posicion positiva, el eje sube, y las hojas se abren

*/
#define IN1  17
#define IN2  16
#define IN3  4
#define IN4  2

#define pd_1 34
#define pd_2 32
#define pd_3 33
int luminosity;

void get_luminosity(){
  luminosity = analogRead(pd_1);
  Serial.print("Lum: ");  Serial.println(luminosity);
}

float current_step = 0.0; //tells the current position in steps referenced at the 0º at the start
float steps;              //steps needed to achieve the rev_pos, could be negative
int t = 0;
char input = ' ';
float value = 0;


//for max torque
int step_torque [5][4] =
{
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1},
  {0, 0, 0, 0}
};

//for equilibrate torque/consume
int step_eq [5][4] =
{
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {1, 0, 0, 1},
  {0, 0, 0, 0}
};

void position(float rev_pos){
  //calculate the steps needed to do
  steps = 0;
  steps = (rev_pos/1000.0) * 2048.0 - current_step;
  steps = round(steps);

  Serial.print("Steps needed: ");
  Serial.println(steps);
  delay(100);

  if(steps>0){ //___________________________________counter clock wise - positive___________________________________
    Serial.println("enter the postive steps");
    t = 0;  
    while(t < steps){
      for (int i = 0; i < 4; i++){
        digitalWrite(IN1, step_eq[i][0]);
        digitalWrite(IN2, step_eq[i][1]);
        digitalWrite(IN3, step_eq[i][2]);
        digitalWrite(IN4, step_eq[i][3]);
        delay(15);
        //Serial.println(t);
        t++;
      }
    }    

  }else if(steps<0){ //______________________________________clock wise - negative______________________________________
    Serial.println("enter the negative steps");
    t=0;
    while(t > steps){ //steps is a negative value
      for (int i = 3; i>=0; i--){
        digitalWrite(IN1, step_eq[i][0]);
        digitalWrite(IN2, step_eq[i][1]);
        digitalWrite(IN3, step_eq[i][2]);
        digitalWrite(IN4, step_eq[i][3]);
        delay(15);
        //Serial.println(t);
        t--;
      }
    }
  }

  current_step+=t;
  t=0;
  Serial.print("Finish -> ");
  Serial.print("Current step: ");
  Serial.println(current_step);
  delay(100);

  //turn of all the bobines
  digitalWrite(IN1, step_eq[4][0]);
  digitalWrite(IN2, step_eq[4][1]);
  digitalWrite(IN3, step_eq[4][2]);
  digitalWrite(IN4, step_eq[4][3]); 
}
 
void setup(){
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);  pinMode(IN2, OUTPUT);  pinMode(IN3, OUTPUT);  pinMode(IN4, OUTPUT);
}
 
void loop(){ 
  get_luminosity();
  position(map(luminosity,0, 4095, 0, 130));
  delay(1000);

  position(0); position(130);

  /*
  position(0.14);
  delay(500);
  position(0);
  delay(500);
  if(Serial.available() != 0){
    input = Serial.read();
    if(input == '+'){
      value += 0.01;
    }
    if(input == '-'){
      value = value-0.01;
    }
    if(input == 'R'){
      value = 0;
    }
    if(input == '1'){
      value += 0.05;
    }
    if(input == '2'){
      value -= 0.05;
    }
    if(input == 'E'){
      value += 1;
    }
    if(input == 'F'){
      current_step = 0;
    }
    Serial.print("Value"); Serial.println(value);
    position(value);
  }
  */
}