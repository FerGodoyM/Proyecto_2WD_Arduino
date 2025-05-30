

#include <Servo.h>

Servo servoMotor;

void setup() {
  Serial.begin(9600);
  servoMotor.attach(10);
}

void loop() {
  //inicia en 5 grados debido a que es la posicion inicial deseada
  servoMotor.write(55);
  delay(1000);
  

  //pasa a posicion 100 que seria el frente en la realidad
  servoMotor.write(100);
  delay(1000);

  servoMotor.write(145);
  delay(1000);

}

