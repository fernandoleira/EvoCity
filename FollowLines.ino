/*

      --- © Copyright 2016. Alberto no lo puede ver ---

*/
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

// Velocidad estandar, constante, giro, tiempo de espera
const int vel_base = 50;
int kp = 28.75;
double gr = 0;
long timeOfD = 1700;

// Ultima distancia del ultrasonido
float ultimaD = 20;
long init_time = 0;
// Leer el siguelineas
int errorA;
int errorB;
// Principio del programa
bool start = true;
// Fin del programa
bool pEnd = false;
int ap = 0;
// Rotonda
bool inRT = false;
long ultimaSig = -timeOfD;

void setup() {
  Serial.begin(9600);
  digitalWrite(12, HIGH);
  for (int i = 0; i < 10; i++) {
    initUs();
    delay(100);
  }
  digitalWrite(12, LOW);

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

  attachInterrupt(0, interruptRising, RISING); // pin 2
  //entrarRotonda();
}

void loop() {
  //initUs();
  //delay(100);
  //Serial.println(ultimaD);

  // Leer el sensor siguelineas
  int err2 = leerSenal();
  Serial.println(err2);
  if(err2 == 1 && ultimaSig+timeOfD < millis()){
    gr = -1.5;
    //vel_base = 35;
    ultimaSig = millis();
    playNote('e',timeOfD);
  }
  if(err2 == -1 && ultimaSig+timeOfD < millis()){
    gr = 1.5;
    //vel_base = 35;
    ultimaSig = millis();
    playNote('c',timeOfD);
  }
  if(err2 == 360 && (ultimaSig+timeOfD < millis() || ultimaSig+300 > millis())){
    playNote('C',timeOfD);
    moveMs(100,100);
    delay(1000);
    while(digitalRead(inzIP)== HIGH && digitalRead(inzDP)==HIGH){
      delay(10);
    }
  }
  /*if(err2 == 361 && errorB == 0){
    stopM();
    delay(10000);
  }*/
  if(err2 == 0 && ultimaSig+timeOfD < millis()){
    gr=0;
    //vel_base = 50;
  }
  double err = leerError() + gr;
  /*if (ultimaD <= 12.00 && ultimaD > 3 && ap != 0) {
    stopM();
    playNote('c',200);
    delay(200);
  } */
  
  int vel_der = vel_base + err*kp;
  if(vel_der < 0)vel_der = 0;    
  int vel_izq = vel_base - err*kp;
  if(vel_izq < 0)vel_izq = 0; 
  
  moveMs(vel_der,vel_izq);
  
  Serial.println(vel_base);
  //ap++;
}

int leerError() {
  int inzI = digitalRead(inzIP);
  int inzD = digitalRead(inzDP);

  if (inzI == 0 && inzD == 0) {
    errorA = 0;
    return 0;
  }
  else if (inzI == 0 && inzD == 1) {
    errorA = 1;
    return 1;
  }
  else if (inzI == 1 && inzD == 0) {
    errorA = -1;
    return -1;
  }
  else {
    if (errorA == 1) {
      return 2;
    }
    else {
      return -2;
    }
  }
}

int leerSenal() {
  bool inzI = digitalRead(inzIP);
  bool inzD = digitalRead(inzDP);
  bool inzII = digitalRead(inzIP2);
  bool inzDD = digitalRead(inzDP2);


  if (inzII == LOW && inzDD == LOW && (inzI == LOW || inzD == LOW)) {
    errorB = 360;
    return 360;
  }
  /*if (inzII == HIGH && inzDD == HIGH && inzI == HIGH && inzD == HIGH) {
    return 361;
  }*/
  if (inzDD == LOW && inzII == HIGH && (inzI == LOW || inzD == LOW)) {
    errorB = 1;
    return 1;
  }
  if (inzII == LOW && inzDD == HIGH && (inzI == LOW || inzD == LOW)) {
    errorB = -1;
    return -1;
  }
  
  else {
    errorB = 0;
    return 0;
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
  //playNote('c',1000);
}

// Iniciar el sensor ultrasonido
void initUs() {
  digitalWrite(pinTri, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTri, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTri, LOW);
  //pulseIn(pinEch);
}

void interruptRising() {
  attachInterrupt(0, interruptFalling, FALLING);
  init_time = micros();
}

void interruptFalling() {
  attachInterrupt(0, interruptRising, RISING);
  long ancho_pul = micros() - init_time;
  ultimaD = ancho_pul / 29 / 2;
}


void playTone(int tonee, int duration) {
  tone(zum, tonee, duration);
 /* for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(zum, HIGH);
    delayMicroseconds(tone);
    digitalWrite(zum, LOW);
    delayMicroseconds(tone);
  }*/
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 523, 554, 587, 622, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

