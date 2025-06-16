// CONFIGURACION 2WD ESPACIOS ESTRECHOS

#include <Servo.h>
#define ANGULO_IZQ 0
#define ANGULO_CEN 77
#define ANGULO_DER 172

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

void girarDerecha() {
  // Motor derecho gira hacia atrás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Motor izquierdo gira hacia adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void girarIzquierda() {
  // Motor derecho gira hacia adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor izquierdo gira hacia atrás
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
  delay(500);  // Deja que el servo se estabilice

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

int contadorRetroceder= 0; 
void loop() {
  distanciaCen = medirDistanciaEn(ANGULO_CEN);
  Serial.print("Centro: ");
  Serial.print(distanciaCen);

  if(distanciaCen >= 13){
    avanzar();
    contadorRetroceder = 0;
  }else{
    detener();
    distanciaDer = medirDistanciaEn(ANGULO_DER);
    distanciaIzq = medirDistanciaEn(ANGULO_IZQ);
    if(distanciaDer >= 13 && distanciaDer > distanciaIzq){
      girarDerecha();
      delay(180);
      contadorRetroceder = 0;
    }else if (distanciaIzq >= 13 && distanciaIzq > distanciaDer) {
      girarIzquierda();
      delay(180);
      contadorRetroceder = 0;
    }else{
      retroceder();
      delay(500);
      contadorRetroceder += 1;
    }
  }

  if(contadorRetroceder == 3){
    retroceder();
    detener();
    distanciaCen = medirDistanciaEn(ANGULO_CEN);
    distanciaDer = medirDistanciaEn(ANGULO_DER);
    distanciaIzq = medirDistanciaEn(ANGULO_IZQ);

    if(distanciaCen > distanciaIzq && distanciaCen > distanciaDer){
      avanzar();
    }
    else if(distanciaIzq > distanciaCen && distanciaIzq > distanciaDer){
      girarIzquierda();
      delay(180);
    }else if(distanciaDer > distanciaIzq && distanciaDer > distanciaCen){
      girarDerecha();
      delay(180);
    }
    contadorRetroceder = 0;
  }
}

