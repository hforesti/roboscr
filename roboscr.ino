#define CLK 9 //CLOCK 
#define ENB 10 //ENABLE 
#define DIR 8 //DIRECTION 

bool clkNvl = 0; //Nível do sinal de Clock 
long timeStamp = micros();
long timeClk = 100;
int stepToDo = 0;
char velMode = 'N'; //N normal, A acelera, D desacelera

void runMotor(void){
  if ((stepToDo) || (velMode == 'C')) {
    if ((micros() - timeStamp) >= timeClk){
      timeStamp = micros();
      clkNvl = !clkNvl;
      digitalWrite(CLK,clkNvl);
      if (!clkNvl){
        stepToDo --;
      }
      if (velMode == 'A'){
        timeClk = map(stepToDo,1,10000,50,500);
      }
      if (velMode == 'D'){
        timeClk = 550 - map(stepToDo,1,10000,50,500);
      }
    }
  }else{
    digitalWrite(ENB,LOW);
  }          
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start");
  pinMode( CLK , OUTPUT);
  pinMode( ENB , OUTPUT);
  pinMode( DIR , OUTPUT);
  digitalWrite(CLK,clkNvl);
  digitalWrite(ENB, LOW);
  digitalWrite(DIR, LOW);  
}

void loop() {
  String serialIn;
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    serialIn = Serial.readString();
    if (serialIn.substring(0,3) == "C2N"){
      stepToDo = 10000;
      digitalWrite(ENB,LOW);
      velMode = 'N';
    }  
    if (serialIn.substring(0,3) == "C2A"){
      stepToDo = 10000;
      velMode = 'A';
      digitalWrite(ENB,HIGH);
    } 
    if (serialIn.substring(0,3) == "C2D"){
      stepToDo = 10000;
      digitalWrite(ENB,HIGH);
      velMode = 'D';
    }  
    if (serialIn.substring(0,3) == "COH"){
      digitalWrite(DIR,HIGH);
      digitalWrite(ENB,HIGH);
      Serial.println(" Rodando contínuo Horário ");     
      velMode = 'C';
    } 
    if (serialIn.substring(0,3) == "COA"){
      digitalWrite(DIR,LOW);
      digitalWrite(ENB,HIGH);
      Serial.println(" Rodando contínuo Anthorário ");
      velMode = 'C';
    }     
    if (serialIn.substring(0,3) == "STP"){
      Serial.println(" Parado ");      
      velMode = 'S';
      stepToDo = 0;
    }  
  } 
  runMotor();
}
