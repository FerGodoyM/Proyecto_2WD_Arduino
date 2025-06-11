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

const int PWM_IZQ = 3;
const int PWM_DER = 11;


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

const float DISTANCIA_MIN = 20.0;

void controlarMotor(int pin1, int pin2, int pinPWM, int velocidad) {
  if (velocidad > 0) {
    analogWrite(pinPWM, velocidad);
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else if (velocidad < 0) {
    analogWrite(pinPWM, -velocidad);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  } else {
    analogWrite(pinPWM, 0);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
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

  int velocidadMax = 255;
  int velocidadIzq = velocidadMax;
  int velocidadDer = velocidadMax;

  if (distanciaCen < DISTANCIA_MIN) {
    velocidadIzq = 0;
    velocidadDer = 0;
  } else {
    if (distanciaIzq < DISTANCIA_MIN) {
      velocidadIzq = 0;
    }
    if (distanciaDer < DISTANCIA_MIN) {
      velocidadDer = 0;
    }
    velocidadIzq = map(constrain(distanciaIzq, 0, DISTANCIA_MIN*2), 0, DISTANCIA_MIN*2, 0, velocidadMax);
    velocidadDer = map(constrain(distanciaDer, 0, DISTANCIA_MIN*2), 0, DISTANCIA_MIN*2, 0, velocidadMax);

    if (distanciaIzq > DISTANCIA_MIN) velocidadIzq = velocidadMax;
    if (distanciaDer > DISTANCIA_MIN) velocidadDer = velocidadMax;
  }
  controlarMotor(IN1, IN2, PWM_DER, velocidadDer);
  controlarMotor(IN3, IN4, PWM_IZQ, velocidadIzq);

  delay(100);
}

