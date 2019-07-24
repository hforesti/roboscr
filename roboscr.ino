#define NUMMOTORS 3 
#define TIMEVELMAX 20    //Tempo em Microsegundos para clock em velocidade máxima
#define TIMEVELMIN 150  //Tempo em Microsegundos para clock em velocidade mínima

int CLK[NUMMOTORS]={3,6,9};
int DIR[NUMMOTORS]={4,7,10};
int ENB[NUMMOTORS]={5,8,11};

int contMotors = 0; 
bool clkNvl[NUMMOTORS]; //Nível do sinal de Clock 
long timeStamp = micros();
long timeClk[NUMMOTORS];
int stepToDo[NUMMOTORS];
int motorPosition[NUMMOTORS];
int stepNumber[NUMMOTORS];//número de passos que vai dar é igual a stepToDo quando começa mas ele se mantém

void runMotor(void){
  if (stepToDo[contMotors]) {
    digitalWrite(ENB[contMotors],LOW);
    if ((micros() - timeStamp) >= timeClk[contMotors]){
      if ((stepNumber[contMotors] - stepToDo[contMotors]) < (stepNumber[contMotors]/3)){ //MODO ACELERA
        timeClk[contMotors] = map(stepToDo[contMotors],(2*stepNumber[contMotors]/3),stepNumber[contMotors],TIMEVELMAX,TIMEVELMIN);
      }else{
        if ((stepNumber[contMotors] - stepToDo[contMotors]) < (2*stepNumber[contMotors]/3)){ // MODO NORMAL = VELOCIDADE MÁXIMA
          timeClk[contMotors] = TIMEVELMAX;
        }else{
          timeClk[contMotors] = (TIMEVELMAX+TIMEVELMIN) - map(stepToDo[contMotors],1,stepNumber[contMotors]/3,TIMEVELMAX,TIMEVELMIN);//MODO DESACELERA
        }
      }
      timeStamp = micros();
      clkNvl[contMotors] = !clkNvl[contMotors];
      digitalWrite(CLK[contMotors],clkNvl[contMotors]);
      if (!clkNvl[contMotors]){
        stepToDo[contMotors] --;
      }
    }
  }else{
    digitalWrite(ENB[contMotors],HIGH);
  }
  contMotors ++;
  if (contMotors >= NUMMOTORS) contMotors = 0;            
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (contMotors = 0; contMotors < NUMMOTORS;contMotors ++ ){
    clkNvl[contMotors] = 0;
    timeClk [contMotors] = 100;
    stepToDo [contMotors] = 0;
    motorPosition [contMotors] = 0;
    stepNumber [contMotors] = 0;
    Serial.print("Start Motor => ");
    Serial.println(contMotors);
    pinMode( CLK[contMotors] , OUTPUT);
    pinMode( ENB[contMotors] , OUTPUT);
    pinMode( DIR[contMotors] , OUTPUT);
    digitalWrite(CLK[contMotors],clkNvl[contMotors]);
    digitalWrite(ENB[contMotors], HIGH);
    digitalWrite(DIR[contMotors], LOW); 
  } 
}

void loop() {
  String serialIn;
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    serialIn = Serial.readString();
    if (serialIn.substring(0,3) == "C2N"){
      stepToDo[2] = 10000;
      stepNumber[2] = 10000;
    }  
    if (serialIn.substring(0,3) == "C2A"){
      stepToDo[0] = 10000;
      stepNumber[0] = 10000;    } 
    
  } 
  runMotor();
}
