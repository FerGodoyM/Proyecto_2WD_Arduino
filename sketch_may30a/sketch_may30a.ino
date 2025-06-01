

#include <Servo.h>

Servo servoMotor;

//pines del sensor HC-SR04
const int trigPin = 8;
const int echoPin = 9;

//variables para almacenar la distancia medida
long duracion;
float distanciaIzq;
float distanciaCen;
float distanciaDer;


// Variables pensadas para un uso futuro.
// Almacenan una estimación de la distancia en cada dirección (izquierda, centro y derecha)
// mientras el auto esté orientado hacia otro lado o tomando otra lectura.
// Esto permitirá mantener un "mapa aproximado" del entorno para tomar decisiones
// incluso cuando no se estén midiendo directamente esas direcciones.

// float distanciaIzqAprox;
// float distanciaCenAprox;
// float distanciaDerAprox;

void setup() {
  //configuracion de pines
  pinMode(trigPin, OUTPUT);   //salida sensor HC-SR04
  pinMode(echoPin, INPUT);    //Entrada sensor HC-SR04

  Serial.begin(9600);
  servoMotor.attach(10);
}

void loop() {
  //inicia en 5 grados debido a que es la posicion inicial deseada
  servoMotor.write(55);
  delay(500);
  //se asegura que HC-SR04 este en low
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  //se envia un pulso ultrasonico de 10ms
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //para calcular la distancia se multiplica la duracion
  //por la velocida del sonido dividido en 2
  duracion = pulseIn(echoPin, HIGH);
  distanciaIzq = duracion * 0.0343 / 2;
  
  Serial.print("Izquierda: ");
  Serial.print(distanciaIzq);
  Serial.print(" cm | Centro: ");

  //pasa a posicion 100 que seria el frente en la realidad
  //y se repite lo hecho con el sensor HC-SR04
  servoMotor.write(100);
  delay(500);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracion = pulseIn(echoPin, HIGH);
  distanciaCen = duracion * 0.0343 / 2;

  Serial.print(distanciaCen);
  Serial.print(" cm | Derecha: ");

  //pasa a posicion 145 que seria 135 grados en la realidad
  //y se repite lo hecho con el sensor HC-SR04
  servoMotor.write(145);
  delay(500);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracion = pulseIn(echoPin, HIGH);
  distanciaDer = duracion * 0.0343 / 2;

  Serial.print(distanciaDer);
  Serial.println(" cm");
}

