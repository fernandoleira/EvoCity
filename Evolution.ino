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

Servo motorR;
Servo motorL;
Servo motorU;

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
  while (pEnd == false) {
    initUs();
    delay(100);
    Serial.println(ultimaD);

    // Leer el sensor siguelineas

    int err = leerError();
    int err2 = leerError2();
    /*if (ultimaD <= 12.00 && ultimaD > 3 && ap != 0) {
      err = 10;
    } */
    // Casos de movimiento
    switch (err) {
      // Recto
      case 0:
        moveMs(100, 100);
        break;
      // Izquierda
      case 1:
        moveMs(100, 15);
        break;
      // Izquierda +
      case 2:
        moveMs(100, 5);
        break;
      // Derecha
      case -1:
        moveMs(15, 100);
        break;
      // Derecha +
      case -2:
        moveMs(5, 100);
        break;
      // Parar
      case 10:
        stopM();
        playNote('c',200);
        delay(200);
        break;     
    }
    ap++;
  }
  pEnd = true;
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

int leerError2() {
  int inzI = digitalRead(inzIP2);
  int inzD = digitalRead(inzDP2);

  if (inzI == 0 && inzD == 0) {
    errorB = 0;
    return 0;
  }
  else if (inzI == 0 && inzD == 1) {
    errorB = 1;
    return 1;
  }
  else if (inzI == 1 && inzD == 0) {
    errorB = -1;
    return -1;
  }
  else {
    if (errorB == 1) {
      return 2;
    }
    else {
      return -2;
    }
  }
}



// value=[-100,100]
void moveMs(int value1, int value2) {
  if(value1>100)value1=100;
  if(value2>100)value2=100;
  if(value1<-100)value1=-100;
  if(value2<-100)value2=-100;
  int velL =  map(value1, -100, 100, 0, 180 );
  int velR =  map(-value2, -100, 100, 0, 180 );

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


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(zum, HIGH);
    delayMicroseconds(tone);
    digitalWrite(zum, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

