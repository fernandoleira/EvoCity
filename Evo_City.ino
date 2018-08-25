/*******************************
 * Evo_City.ino
 * Author: Fernando Leira
*******************************/

void parking() {
  moveMs(80, -80);
  delay(300);
  moveMs(-80, -80);
  delay(1200);
  moveMs(-80, 80);
  delay(300);
  moveMs(100, 100);
  delay(500);
  moveMs(0, 0);
}

void enter_roundabot() {
  int speeds[] = {140, 90, 40};
  stopM();
  for(int i = 0; i<3; i++){
    motorU.write(speeds[i]);
    delay(200);
    initUs();
    delay(100);
    while (ultimaD <= 40.00 && ultimaD > 3) {
      stopM();
      playNote('c',200);
      initUs();
      delay(100);
    }
  }
  motorU.write(90);
  inRT=true;
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

// Sonidos
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

