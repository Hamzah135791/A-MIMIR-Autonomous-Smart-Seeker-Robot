#include <Servo.h>
#include "NewPing.h"
#include "AFMotor.h"

Servo myServo;
int pos = 0;

#define TRIGGER_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 400
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int MOTOR_1 = 1;
const int MOTOR_2 = 2;
const int MOTOR_3 = 3;
const int MOTOR_4 = 4;

AF_DCMotor motor1(MOTOR_1, MOTOR12_64KHZ);
AF_DCMotor motor2(MOTOR_2, MOTOR12_64KHZ);
AF_DCMotor motor3(MOTOR_3, MOTOR12_64KHZ);
AF_DCMotor motor4(MOTOR_4, MOTOR12_64KHZ);

void servoSweep(int start, int end, int step);
long scanning();
void motorControl(int m1, int m2, int m3, int m4, int lama);
void berhenti();
void maju(int lama);
void mundur(int lama);
void kanan(int lama);
void kiri(int lama);


const int ldrPin = A2;    // Pin analog tempat LDR terhubung
const int ledPin1 = 7;    // Pin digital untuk LED pertama
const int ledPin2 = 2;    // Pin digital untuk LED kedua
const int threshold = 700; // Batas intensitas cahaya untuk LDR
int ldrValue;             // Variabel untuk menyimpan nilai LDR


void setup() {
  Serial.begin(9600);
  myServo.attach(10);

  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor4.setSpeed(150);

  pinMode(ledPin1, OUTPUT);     // Atur pin LED pertama sebagai output
  pinMode(ledPin2, OUTPUT);     // Atur pin LED kedua sebagai output
}

void loop() {
  ldrValue = analogRead(ldrPin);  // Baca nilai LDR (ADC conversion)
  Serial.print("Nilai LDR: ");
  Serial.println(ldrValue);      // Tampilkan nilai LDR di Serial Monitor

  if (ldrValue >= threshold) {
    // Jika cahaya cukup terang, LED berkedip seperti lampu sein
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    delay(500);                  // Tunggu 500 ms
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(500);                  // Tunggu 500 ms
  } else {
    // Jika cahaya agak gelap, kedua LED menyala konstan
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  }


  // long jarakDepan = scanning();

  // // Jika jarak depan kurang dari threshold, periksa kanan dan kiri
  // if (jarakDepan < 30) {
  //   servoSweep(30, 150, 1);  // Sweep ke kanan
  //   long jarakKanan = scanning();

  //   servoSweep(150, 30, -1);  // Sweep ke kiri
  //   long jarakKiri = scanning();

  //   // Pilih arah berdasarkan jarak
  //   if (jarakKanan > 30 && jarakKanan > jarakKiri) {
  //     kanan(1000);
  //   } else if (jarakKiri > 30) {
  //     kiri(1000);
  //   } else {
  //     mundur(1000);  // Jika kanan dan kiri tidak memungkinkan
  //   }
  // } else {
  //   maju(1000);  // Jika jalan depan aman
  // }
}

void servoSweep(int start, int end, int step) {
  for (pos = start; pos != end; pos += step) {
    myServo.write(pos);
    delay(15);
  }
}

long scanning() {
  long jarak = sonar.ping_cm();

  // Coba baca ulang jika hasil 0
  if (jarak == 0) {
    delay(50);  // Tunggu sebentar sebelum membaca ulang
    jarak = sonar.ping_cm();
  }

  // Jika tetap 0, anggap jarak maksimum
  if (jarak == 0) jarak = MAX_DISTANCE;

  return jarak;
}

void motorControl(int m1, int m2, int m3, int m4, int lama) {
  motor1.run(m1);
  motor2.run(m2);
  motor3.run(m3);
  motor4.run(m4);
  delay(lama);
  berhenti();
}

void berhenti() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void maju(int lama) {
  motorControl(FORWARD, FORWARD, FORWARD, FORWARD, lama);
}

void mundur(int lama) {
  motorControl(BACKWARD, BACKWARD, BACKWARD, BACKWARD, lama);
}

void kanan(int lama) {
  motorControl(FORWARD, BACKWARD, FORWARD, BACKWARD, lama);
}

void kiri(int lama) {
  motorControl(BACKWARD, FORWARD, BACKWARD, FORWARD, lama);
}