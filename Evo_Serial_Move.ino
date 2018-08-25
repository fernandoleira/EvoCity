#include <Servo.h>

// Declaramos Pins
const int zum = 8;
const int inzIP = 4;
const int inzDP = 3;
const int inzIP2 = 10;
const int inzDP2 = 11;
const int pinM_I = 6;
const int pinM_D = 5;
const int pinM_U = 12;
const int pinTri = 9;
const int pinEch = 2;

Servo motorR;
Servo motorL;
Servo motorU;

char let = '0';
String ultimoComando = "";

void setup() {
  Serial.begin(19200);

  motorR.attach(pinM_D);
  motorL.attach(pinM_I);
  motorU.attach(pinM_U);
  pinMode(zum, OUTPUT);
  pinMode(inzIP, INPUT);
  pinMode(inzDP, INPUT);
  pinMode(inzIP2, INPUT);
  pinMode(inzDP2, INPUT);
  pinMode(pinTri, OUTPUT);
  pinMode(pinEch, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
   leerComando();
   Serial.println(ultimoComando);
   
   if(ultimoComando == "delante"){ 
     moveMs(100,100);
   } 
   else if(ultimoComando == "detras"){
     moveMs(-100,-100);
   }
   else if(ultimoComando == "izquierda"){
     moveMs(100,-100);
   }
   else if(ultimoComando == "derecha"){
     moveMs(-100,100);
   }
   else if(ultimoComando == "parar"){
     moveMs(0,0);
   }
}

// value=[-100,100]
void moveMs(int value1, int value2) {
  if(value1>100)value1=100;
  if(value2>100)value2=100;
  if(value1<-100)value1=-100;
  if(value2<-100)value2=-100;

  int velL =  map(value1, -100, 100, 0, 180);
  int velR =  map(-value2, -100, 100, 0, 180);

  motorL.write(velL);
  motorR.write(velR);
}

void stopM() {
  motorL.write(90);
  motorR.write(90);
}

void leerComando(){
  int i;
  int numLetras = Serial.available();
  for(i=0;i<numLetras;i++){
    char letra = (char)Serial.read();
    if(letra == '='){
      ultimoComando = "";
    }
    else if(letra == '+'){
      return;
    }
    else {
      ultimoComando = ultimoComando + letra;
    }
  }
}
