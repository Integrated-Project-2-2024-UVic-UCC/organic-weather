#define IN1  14
#define IN2  27
#define IN3  26
#define IN4  25

float current_step = 0.0; //tells the current position in steps referenced at the 0º at the start
float steps;              //steps needed to achieve the rev_pos, could be negative
int t = 0;
float revs;
max_rev = 2;

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

int sensor1 = 12;
int light_value;

void position(float rev_pos){
  //calculate the steps needed to calcular quants pasos ha de fer referent a la posicio actual i el sentit de gir
  steps = 0;
  steps = rev_pos * 2048.0 - current_step;

  Serial.print("Steps: ");
  Serial.println(steps);
  delay(3000);

  if(steps>0){ //___________________________________counter clock wise - positive___________________________________
    t = 0;  
    
    while(t < steps){
      for (int i = 0; i < 4; i++){
        digitalWrite(IN1, step_eq[i][0]);
        digitalWrite(IN2, step_eq[i][1]);
        digitalWrite(IN3, step_eq[i][2]);
        digitalWrite(IN4, step_eq[i][3]);
        delay(10);
        Serial.println(t);
        t++;
      }
    }    
    current_step+=t;
    t=0;
    Serial.print("Current step: ");
    Serial.println(current_step);
    delay(1000);
  }


  if(steps<0){ //______________________________________clock wise - negative______________________________________
    t=0;
    while(t > steps){ //steps is a negative value
      for (int i = 3; i>=0; i--){
        digitalWrite(IN1, step_eq[i][0]);
        digitalWrite(IN2, step_eq[i][1]);
        digitalWrite(IN3, step_eq[i][2]);
        digitalWrite(IN4, step_eq[i][3]);
        delay(10);
        Serial.println(t);
        t--;
      }
    }
    current_step+=t;
    t=0;
    Serial.print("Current step: ");
    Serial.println(current_step);
    delay(1000);
  }

  //turn off all the bobines
  digitalWrite(IN1, step_eq[4][0]);
  digitalWrite(IN2, step_eq[4][1]);
  digitalWrite(IN3, step_eq[4][2]);
  digitalWrite(IN4, step_eq[4][3]); 
}
 
void setup(){
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);  pinMode(IN2, OUTPUT);  pinMode(IN3, OUTPUT);  pinMode(IN4, OUTPUT);
}
 
void loop(){ 
  //cicle principal de mesura de llum i obertura de petals
  light_value=analogRead(sensor1);
  Serial.println(light_value);
  revs=(map(light_value,0,4095,0,1000*max_rev))/1000.0;
  Serial.println(revs);
  position(revs);
}