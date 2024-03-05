#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11

float current_step = 0.0; //tells the current position in steps referenced at the 0º at the start
float steps;              //steps needed to achieve the rev_pos, could be negative
int t = 0;

//for max torque
int step_torque [4][4] =
{
  {1, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 1},
  {1, 0, 0, 1}
};

//for equilibrate torque/consume
int step_eq [4][4] =
{
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1,0},
  {1, 0, 0, 1}
};

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
}
 
void setup(){
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);  pinMode(IN2, OUTPUT);  pinMode(IN3, OUTPUT);  pinMode(IN4, OUTPUT);
}
 
void loop(){ 
  Serial.println("Posicio 0.25");
  position(0.25);
  delay(500);

  Serial.println("Posicio 0");
  position(0.0);
  delay(500);

}