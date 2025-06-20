// CONFIGURACION 2WD ESPACIOS AMPLIOS

#include <Servo.h>

Servo servoMotor;

#define ANGULO_IZQ 20
#define ANGULO_CEN 77
#define ANGULO_DER 125

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

float histIzq[3] = {-1, -1, -1};
float histCen[3] = {-1, -1, -1};
float histDer[3] = {-1, -1, -1};

int repeticionesSimilares = 0;
const int repeticionesLimite = 3;
const float tolerancia = 3.0;


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

void retroceder(){
  digitalWrite(IN1, LOW);   // Motor derecho atrás
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);  // Motor izquierdo atrás
  digitalWrite(IN4, LOW);
}

// ------------------- FUNCIONES -------------------------

float medirDistanciaEn(int angulo) {
  servoMotor.write(angulo);
  delay(215);  // Deja que el servo se estabilice

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.0343 / 2;  // Devuelve la distancia en cm
}

bool sonSimilares(float nueva, float h1, float h2, float h3) {
  return abs(nueva - h1) < tolerancia &&
         abs(nueva - h2) < tolerancia &&
         abs(nueva - h3) < tolerancia;
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

// Para verificar que tomo una decision
  bool accionTomada = false;
 // 1. Camino completamente libre
  if(distanciaCen >= 25 && distanciaIzq > 15 && distanciaDer > 15){
    avanzar();
    accionTomada = true;
  }
  // 2. Lateral muy cerca, corregir dirección
  else if(distanciaCen >= 25 && distanciaIzq < 15){
    doblarDerecha();
    delay(200);
    accionTomada = true;
  }else if(distanciaCen >= 25 && distanciaDer < 15){
    doblarIzquierda();
    delay(200);
    accionTomada = true;
  // 3. Centro libre, pero un lado es claramente más despejado
  }else if(distanciaCen > 25 && distanciaDer > distanciaCen && distanciaDer > distanciaIzq){
    doblarDerecha();
    delay(400);    
    accionTomada = true;

  }else if(distanciaIzq > 25 && distanciaIzq > distanciaCen && distanciaIzq > distanciaDer){
    doblarIzquierda();
    delay(400);
    accionTomada = true;

  // 4. Atascado por completo (menos de 20 cm en todos lados)
  }else if(distanciaCen < 20 && distanciaIzq < 20 && distanciaDer < 20){
    retroceder();
    delay(600);
    vuelta();
    delay(800);
    accionTomada = true;

  // 5. Algún obstáculo muy cercano (<10 cm)
  }else if(distanciaCen < 10 || distanciaIzq < 10 || distanciaDer < 10){    
    retroceder();
    delay(600);
    accionTomada = true;

  // 6. Zona intermedia (20–25 cm al frente, laterales bien)
  }else if (distanciaCen >= 20 && distanciaCen < 25 && distanciaIzq > 15 && distanciaDer > 15) {
    avanzar();
    accionTomada = true;

  // 7. Zona estrecha, elegir dirección más despejada
  }else if (distanciaCen < 25 && distanciaCen >= 15) {
    if (distanciaDer > distanciaIzq && distanciaDer > 20) {
      doblarDerecha();
      delay(300);
    } else if (distanciaIzq > distanciaDer && distanciaIzq > 20) {
      doblarIzquierda();
      delay(300);
    } else {
      retroceder();
      delay(300);
    }
    accionTomada = true;
  }


  //Se habia implementado un ELSE para las condiciones no
  //tomadas, que simplemente retrocedia. pero aun asi encontre una situacion
  //en donde el auto quedo completamente estatico
  //por eso mismo implemente los 2 condicionales que siguen

  // Si ninguna condición se cumplió
  if (!accionTomada) {
    Serial.println("Sin condición clara. Retrocediendo por seguridad.");
    retroceder();
    delay(600);
  }

  // Verificación de que los motores no estén apagados
  if (digitalRead(IN1) == LOW && digitalRead(IN2) == LOW &&
      digitalRead(IN3) == LOW && digitalRead(IN4) == LOW) {
    Serial.println("Motores apagados detectados. Retrocediendo preventivamente.");
    retroceder();
    delay(500);
  }


  if (sonSimilares(distanciaIzq, histIzq[0], histIzq[1], histIzq[2]) &&
    sonSimilares(distanciaCen, histCen[0], histCen[1], histCen[2]) &&
    sonSimilares(distanciaDer, histDer[0], histDer[1], histDer[2])) {

  repeticionesSimilares++;
  Serial.println(" Distancias similares detectadas.");
} else {
  repeticionesSimilares = 0; // Reiniciar si hubo cambio
}

// Mover historial (cola FIFO)
  for (int i = 2; i > 0; i--) {
    histIzq[i] = histIzq[i - 1];
    histCen[i] = histCen[i - 1];
    histDer[i] = histDer[i - 1];
  }

  histIzq[0] = distanciaIzq;
  histCen[0] = distanciaCen;
  histDer[0] = distanciaDer;

  // Si el patrón se repite muchas veces, asumir atasco
  if (repeticionesSimilares >= repeticionesLimite) {
    Serial.println("Atasco detectado por repetición. Retrocediendo");
    retroceder();
    delay(500);
    repeticionesSimilares = 0;
  }
}



