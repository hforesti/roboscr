#define CLK 9 //CLOCK 
#define ENB 10 //ENABLE 
#define DIR 8 //DIRECTION 
#define TIMEVELMAX 20    //Tempo em Microsegundos para clock em velocidade máxima
#define TIMEVELMIN 150  //Tempo em Microsegundos para clock em velocidade mínima
 
bool clkNvl = 0; //Nível do sinal de Clock 
long timeStamp = micros();
long timeClk = 100;
int stepToDo = 0;
int motorPosition = 0;
int stepNumber = 0;
char velMode = 'N'; //N normal, A acelera, D desacelera
void runMotor(void){
  if ((stepToDo) || (velMode == 'C')) {
    if ((micros() - timeStamp) >= timeClk){
      if ((stepNumber - stepToDo) < (stepNumber/3)){ //MODO ACELERA
        timeClk = map(stepToDo,(2*stepNumber/3),stepNumber,TIMEVELMAX,TIMEVELMIN);
      }else{
        if ((stepNumber - stepToDo) < (2*stepNumber/3)){ // MODO NORMAL = VELOCIDADE MÁXIMA
          timeClk = TIMEVELMAX;
        }else{
          timeClk = (TIMEVELMAX+TIMEVELMIN) - map(stepToDo,1,stepNumber/3,TIMEVELMAX,TIMEVELMIN);//MODO DESACELERA
        }
      }
      //Serial.print("SteptoDo=>");
      //Serial.print(stepToDo);
      //Serial.print("timeClk=>");
      //Serial.println(timeClk);
      timeStamp = micros();
      clkNvl = !clkNvl;
      digitalWrite(CLK,clkNvl);
      if (!clkNvl){
        stepToDo --;
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
      stepNumber = 10000;
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
