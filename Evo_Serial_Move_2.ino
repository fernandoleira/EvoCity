/*********************************
 * Evo_Serial.ino
 * Author: Fernando Leira
**********************************/

#include <Servo.h>

// Declare Pins
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

// Declare Servo Motors
Servo motorR;
Servo motorL;
Servo motorU;

char let = '0';
String lastCommand = "";


String pX = "0";
String pY = "0"; 

// Setup Function
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

// Loop function
void loop() {
   readCommand();
   Serial.println(pX + ", " + pY);

   int nX = pX.toInt();
   int nY = pY.toInt();
   nX = map(nX, 0, 320, -100, 100); 
   nY = map(nY, 0, 400, -100, 100); 

   moveMs(nX - nY, nX + nY);
}

// Void function to move Servos
// Value Format [-100,100]
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

// Void function to stop Servos
void stopM() {
  motorL.write(90);
  motorR.write(90);
}

// Read commands from Bluethoot Serial
void readCommand(){
  int i;
  int num_letters = Serial.available();
  Serial.print("lenyendo comando de numLetras= ");
  Serial.println(num_letters);
  for(i=0;i<num_letters;i++){
    
    char letter = (char)Serial.read();
    
    if (letter == '{'){
      lastCommand = "";
    }
    else if (letter == '}'){
      pY = lastCommand;
      return;
    }
    else if (letter == ','){
      pX = lastCommand;
      lastCommand = "";
    }
    else {
      lastCommand = lastCommand + letter;
    }
  }
}
