// CONFIGURACION 2WD UNIVERSIDAD

#include <Servo.h>
#define ANGULO_IZQ 55
#define ANGULO_CEN 100
#define ANGULO_DER 145

Servo servoMotor;

//Motor Izquierdo
const int IN1 = 7;
const int IN2 = 6;

//Motor Derecho
const int IN3 = 5;
const int IN4 = 4;

//pines del sensor HC-SR04
const int trigPin = 8;
const int echoPin = 9;

//variables para almacenar la distancia medida
long duracion;
float distanciaIzq = 0;
float distanciaCen = 0;
float distanciaDer = 0;

//   --------------FUNCIONES DE MOVIMIENTO--------------------
// NO CONFIRME EL FUNCIONAMIENTO DE ESTOS MOVIMIENTOS EN EL AUTO DE LA UNIVERSIDAD

void avanzar(){
  //MOTOR DERECHO GIRA PARA ADELANTE
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  //MOTOR IZQUIERDO GIRA PARA ADELANTE
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void detener(){
  //MOTOR DERECHO APAGADO
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  //MOTOR IZQUIERDO APAGADO
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void doblarDerecha(){
    // Derecho detenido
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Izquierdo adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void doblarIzquierda(){
  // Derecho adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Izquierdo detenido
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void vuelta(){
  // Derecho atrás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Izquierdo adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void retroceder(){
  digitalWrite(IN1, LOW);   // Motor derecho atrás
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);  // Motor izquierdo atrás
  digitalWrite(IN4, LOW);
}

// ------------------- FUNCIONES -------------------------

float medirDistanciaEn(int angulo) {
  servoMotor.write(angulo);
  delay(200);  // Deja que el servo se estabilice

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.0343 / 2;  // Devuelve la distancia en cm
}


void setup() {
  //configuracion de pines
  pinMode(trigPin, OUTPUT);   //salida sensor HC-SR04
  pinMode(echoPin, INPUT);    //Entrada sensor HC-SR04

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  servoMotor.attach(10);
}

void loop() {
  distanciaIzq = medirDistanciaEn(ANGULO_IZQ);
  distanciaCen = medirDistanciaEn(ANGULO_CEN);
  distanciaDer = medirDistanciaEn(ANGULO_DER);

  Serial.print("Izquierda: ");
  Serial.print(distanciaIzq);
  Serial.print(" cm | Centro: ");
  Serial.print(distanciaCen);
  Serial.print(" cm | Derecha: ");
  Serial.println(distanciaDer);

  

  
}

