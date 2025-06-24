#include <Servo.h>

#define ANGULO_IZQ 0
#define ANGULO_CEN 77
#define ANGULO_DER 172

Servo servoMotor;

// Motores
const int IN1 = 7;  // Motor derecho
const int IN2 = 6;
const int IN3 = 5;  // Motor izquierdo
const int IN4 = 4;

const int ENA = 3;  // PWM motor derecho
const int ENB = 11; // PWM motor izquierdo


// Sensor HC-SR04
const int trigPin = 8;
const int echoPin = 9;

float distanciaIzq = 0;
float distanciaCen = 0;
float distanciaDer = 0;

// Historial para detección de atasco
float h1 = -1, h2 = -1, h3 = -1;
int repeticionesSimilares = 0;
const int repeticionesLimite = 3;
const float tolerancia = 2.0;

float Kp = 0.8;
float Ki = 0.02;
float Kd = 1.0;

float error = 0;
float integral = 0;
float derivada = 0;
float errorAnterior = 0;
float correccionPID = 0;

int velocidadIzq;
int velocidadDer;
int velocidadBase = 255; 

// -------- FUNCIONES DE MOVIMIENTO --------

void avanzar(int velIzq, int velDer){
  velIzq = constrain(velIzq, 0, 255);
  velDer = constrain(velDer, 0, 255);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, velDer);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, velIzq);
}

void detener(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void girarDerecha() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void girarIzquierda() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void retroceder(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// -------- FUNCIONES AUXILIARES --------

float medirDistanciaEn(int angulo) {
  servoMotor.write(angulo);
  delay(230);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.0343 / 2;
}

bool sonSimilares(float a, float b) {
  return abs(a - b) < tolerancia;
}

void reiniciarPID(){
  integral = 0;
  derivada = 0;
  errorAnterior = 0;
}

// -------- SETUP --------

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  servoMotor.attach(10);
  Serial.begin(9600);
  delay(1000);
}

// -------- LOOP --------

void loop() {
  distanciaCen = medirDistanciaEn(ANGULO_CEN);
  distanciaDer = medirDistanciaEn(ANGULO_DER);
  distanciaIzq = medirDistanciaEn(ANGULO_IZQ);

  bool accionTomada = false;

  // ✅ Zona libre
  if (distanciaCen >= 13) {
    accionTomada = true;

    error = distanciaIzq - distanciaDer;
    integral += error;
    integral = constrain(integral, -100, 100); // Anti-windup
    derivada = error - errorAnterior;
    correccionPID = Kp * error + Ki * integral + Kd * derivada;
    errorAnterior = error;

    velocidadIzq = velocidadBase + correccionPID;
    velocidadDer = velocidadBase - correccionPID;

    velocidadIzq = constrain(velocidadIzq, 0, 255);
    velocidadDer = constrain(velocidadDer, 0, 255);

    avanzar(velocidadIzq, velocidadDer);
  }else {
    reiniciarPID();
  }

  // ✅ Obstruido al frente, buscar lados
  if (!accionTomada && distanciaCen < 13) {
    detener();
    delay(100);

    if (distanciaDer >= 13 && distanciaDer > distanciaIzq) {
      girarDerecha();
      delay(280);
      accionTomada = true;
    } else if (distanciaIzq >= 13 && distanciaIzq > distanciaDer) {
      girarIzquierda();
      delay(280);
      accionTomada = true;
    } else {
      retroceder();
      delay(400);
      accionTomada = true;
    }
  }

  // ✅ Motor parado (doble seguridad)
  if (!accionTomada && digitalRead(IN1) == LOW && digitalRead(IN2) == LOW &&
      digitalRead(IN3) == LOW && digitalRead(IN4) == LOW) {
    retroceder();
    delay(500);
  }

  // ✅ Detección de atasco (historial)
  if (sonSimilares(distanciaCen, h1) &&
      sonSimilares(distanciaCen, h2) &&
      sonSimilares(distanciaCen, h3)) {
    repeticionesSimilares++;
  } else {
    repeticionesSimilares = 0;
  }

  h3 = h2;
  h2 = h1;
  h1 = distanciaCen;

  if (repeticionesSimilares >= repeticionesLimite) {
    retroceder();
    delay(500);
    girarDerecha();
    delay(280);
    repeticionesSimilares = 0;
  }
}
