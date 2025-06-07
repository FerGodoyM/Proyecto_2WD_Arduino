// CONFIGURACION DE MI 2WD

#include <Servo.h>

Servo servoMotor;

#define ANGULO_IZQ 80
#define ANGULO_CEN 120
#define ANGULO_DER 160

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


// Variables pensadas para un uso futuro.
// Almacenan una estimación de la distancia en cada dirección (izquierda, centro y derecha)
// mientras el auto esté orientado hacia otro lado o tomando otra lectura.
// Esto permitirá mantener un "mapa aproximado" del entorno para tomar decisiones
// incluso cuando no se estén midiendo directamente esas direcciones.

// float distanciaIzqAprox;
// float distanciaCenAprox;
// float distanciaDerAprox;

//   --------------FUNCIONES DE MOVIMIENTO--------------------
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

// ------------------- FUNCIONES -------------------------

float medirDistanciaEn(int angulo) {
  servoMotor.write(angulo);
  delay(500);  // Deja que el servo se estabilice

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.0343 / 2;  // Devuelve la distancia en cm
}

// ----------------------- FUNCIONAMIENTO -------------------

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

int estadoMedicion = 0;  // 0=izq, 1=centro, 2=der

void loop() {
  switch(estadoMedicion) {
    case 0:
      distanciaIzq = medirDistanciaEn(ANGULO_IZQ);
      Serial.print("Izquierda: ");
      Serial.print(distanciaIzq);
      Serial.print(" cm | Centro: ");
      estadoMedicion = 1;
      break;
    case 1:
      distanciaCen = medirDistanciaEn(ANGULO_CEN);
      Serial.print(distanciaCen);
      Serial.print(" cm | Derecha: ");
      // Decide aquí según distanciaCen
      if (distanciaCen > 15) {
        avanzar();
      }else {
        detener();}
      estadoMedicion = 2;
      break;
    case 2:
      distanciaDer = medirDistanciaEn(ANGULO_DER);
      estadoMedicion = 0;
      Serial.print(distanciaDer);
      Serial.println(" cm");
      break;
  }
}


