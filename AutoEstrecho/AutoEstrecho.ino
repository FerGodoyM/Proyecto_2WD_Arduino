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

const int ENA = 3;  // PWM motor derechoAdd commentMore actions
const int ENB = 11; // PWM motor izquierdo

//pines del sensor HC-SR04
const int trigPin = 8;
const int echoPin = 9;

//variables para almacenar la distancia medida
long duracion;
float distanciaIzq = 0;
float distanciaCen = 0;
float distanciaDer = 0;

float distancia_min = 20.0;
float distancia_max = 28.0; 

float h1 = -1, h2 = -1, h3 = -1;
int repeticionesSimilares = 0;
const int repeticionesLimite = 3;
const float tolerancia = 2.0;

float Kp = 6;
float Ki = 0.2;
float Kd = 3;

float error = 0;
float integral = 0;
float derivada = 0;
float errorAnterior = 0;
float correccionPID = 0;

int velocidadIzq;
int velocidadDer;
float velocidadControlada;
int velocidadBase = 230;
int velocidadGiro = 200;

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
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  //MOTOR DERECHO APAGADO
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  //MOTOR IZQUIERDO APAGADO
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void girarDerecha() {
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);

  // Motor derecho gira hacia atrás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Motor izquierdo gira hacia adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void girarIzquierda() {
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  // Motor derecho gira hacia adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor izquierdo gira hacia atrás
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void retroceder(){
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  digitalWrite(IN1, LOW);   // Motor derecho atrás
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);  // Motor izquierdo atrás
  digitalWrite(IN4, LOW);
}

// ------------------- FUNCIONES -------------------------

float medirDistanciaEn(int angulo) {
  servoMotor.write(angulo);
  delay(220);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.0343 / 2;
}

void girarHastaDespejar(bool derecha, int maxIntentos = 15) {
  int intentos = 0;
  while (medirDistanciaEn(ANGULO_CEN) < distancia_min + 10.0 && intentos < maxIntentos) {
    if (derecha) {
      girarDerecha();
    } else {
      girarIzquierda();
    }
    analogWrite(ENA, velocidadGiro);
    analogWrite(ENB, velocidadGiro);
    delay(100);
    intentos++;
  }

  if (derecha) {
    girarDerecha();
  } else {
    girarIzquierda();
  }
  analogWrite(ENA, velocidadGiro);
  analogWrite(ENB, velocidadGiro);
  delay(115);
  avanzar();
  analogWrite(ENA, velocidadBase);
  analogWrite(ENB, velocidadBase);
}

bool sonSimilares(float a, float b) {
  return abs(a - b) < tolerancia;
}

void reiniciarPID(){
  integral = 0;
  derivada = 0;
  errorAnterior = 0;
}

void setup() {
  //configuracion de pines
  pinMode(trigPin, OUTPUT);   //salida sensor HC-SR04
  pinMode(echoPin, INPUT);    //Entrada sensor HC-SR04

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  servoMotor.attach(10);
  distanciaCen = medirDistanciaEn(ANGULO_CEN);
  delay(1000);
}

void loop() {
  distanciaCen = medirDistanciaEn(ANGULO_CEN);

  if(distanciaCen >= distancia_min){
    error = distancia_max - distanciaCen;
    integral += error;
    derivada = error - errorAnterior;

    correccionPID = (Kp * error) + (Ki * integral) + (Kd * derivada);
    velocidadControlada = velocidadBase - correccionPID;
    velocidadControlada = constrain(velocidadControlada, velocidadGiro, velocidadBase);
    errorAnterior = error;

    avanzar();
    analogWrite(ENA, velocidadControlada);
    analogWrite(ENB, velocidadControlada);
  }else{
    reiniciarPID();
    distanciaDer = medirDistanciaEn(ANGULO_DER);
    distanciaIzq = medirDistanciaEn(ANGULO_IZQ);

    if(distanciaDer >= distancia_min && distanciaDer > distanciaIzq){
      girarHastaDespejar(true);
    }else if (distanciaIzq >= distancia_min && distanciaIzq > distanciaDer) {
      girarHastaDespejar(false);
    }else{
      if (distanciaDer > distanciaIzq) {
        girarHastaDespejar(true);
      }else {
        girarHastaDespejar(false);
      }
    }
  }

  if (sonSimilares(distanciaCen, h1) &&
      sonSimilares(distanciaCen, h2) &&
      sonSimilares(distanciaCen, h3)) {
    repeticionesSimilares++;
  }else {
    repeticionesSimilares = 0;
  }

  h3 = h2;
  h2 = h1;
  h1 = distanciaCen;

  if (repeticionesSimilares >= repeticionesLimite) {
    retroceder();
    delay(600);
    repeticionesSimilares = 0;
  }
}

