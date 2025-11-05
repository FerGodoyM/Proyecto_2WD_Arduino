# Proyecto 2WD — Robot autónomo para espacios amplios

Descripción
-----------
Este proyecto es un robot 2WD (dos ruedas motrices) controlado por Arduino que se desplaza en espacios amplios evitando colisiones mediante un sensor ultrasónico (HC-SR04) y un servo que barre el sensor para medir a izquierda/centro/derecha.

Características principales
--------------------------
- Movilidad autónoma en espacios abiertos.
- Detección de obstáculos con HC-SR04 montado en un servo.
- Comportamientos: avanzar, girar, retroceder y dar vuelta si está atascado.
- Lógica simple de decisiones basada en umbrales de distancia.

Contenido del repositorio
-------------------------
- `2WD.ino` — Sketch principal de Arduino que implementa la lógica de navegación.

Hardware necesario
------------------
- Arduino (Uno, Nano u otro compatible).
- 2x motores de corriente continua (DC) con rueda.
- Driver de motor H-Bridge (ej. L298N, TB6612 o similar). No conectar los motores directamente al Arduino.
- Sensor ultrasónico HC-SR04.
- Servo (p. ej. SG90) para girar el HC-SR04 y tomar medidas en 3 ángulos.
- Batería o fuente de alimentación para motores (capaz de entregar la corriente requerida).
- Cables, protoboard o placa de montaje.

Mapeo de pines (según `2WD.ino`)
---------------------------------------
Las constantes están definidas en el sketch; conéctalas tal cual o ajusta el sketch si tu hardware difiere.

- Motor (par de pines A): IN1 = 7, IN2 = 6
- Motor (par de pines B): IN3 = 5, IN4 = 4
  - Nota: En el código los pares de pines se usan en las funciones de movimiento; si al montar ves que el robot gira al revés, invierte el orden de uno de los pares o cambia la polaridad en el driver.
- HC-SR04 Trig: pin 8
- HC-SR04 Echo: pin 9
- Servo: pin 10

Sugerencia de conexión con L298N (ejemplo)
-------------------------------------------
- L298N IN1 -> Arduino pin 7
- L298N IN2 -> Arduino pin 6
- L298N IN3 -> Arduino pin 5
- L298N IN4 -> Arduino pin 4
- L298N +12V (o entrada motor) -> fuente de motores (según tus motores)
- L298N GND -> fuente de motores GND y GND común con Arduino
- L298N 5V (si tu módulo tiene regulador) -> NO alimentar el Arduino a menos que sepas lo que haces; lo recomendable es mantener fuentes separadas y un GND común

Conexión HC-SR04
-----------------
- VCC -> 5V
- GND -> GND
- TRIG -> pin 8
- ECHO -> pin 9

Servo
-----
- Señal -> pin 10
- VCC -> 5V (o fuente separada si el servo consume mucho)
- GND -> GND común

Notas de alimentación y seguridad
---------------------------------
- Nunca alimentes los motores desde el pin 5V del Arduino si requieren corrientes altas. Usa una fuente externa para motores y une las masas (GND) con la del Arduino.
- Asegura que el driver de motor soporte la corriente pico de tus motores.

Comportamiento y parámetros importantes
---------------------------------------
En el sketch `2WD.ino` se usan varios umbrales que puedes ajustar:
- Umbral frontal (`distanciaCen`) para considerar el camino libre: 25 cm
- Umbrales laterales para correcciones: 15 cm
- Umbral de emergencia/retroceso: 10 cm
- Distancia para detectar un atasco y ejecutar maniobra de escape: <20 cm (en los tres sensores)

Ángulos del servo (constantes en el sketch)
------------------------------------------
- `ANGULO_IZQ` = 20
- `ANGULO_CEN` = 77
- `ANGULO_DER` = 125

Ajusta estos valores si tu montaje físico requiere otros ángulos.

Subir el sketch
---------------
1. Abre el IDE de Arduino (o Arduino CLI / PlatformIO).
2. Selecciona tu placa (por ejemplo, Arduino Uno) y el puerto COM correspondiente.
3. Abre `2WD.ino` y carga el sketch.

Calibración y pruebas
---------------------
- Primero prueba el servo con un sketch simple que lo mueva a los 3 ángulos para confirmar su orientación.
- Luego verifica las mediciones del HC-SR04 con un sketch que imprima la distancia por Serial para validar lecturas.
- Ajusta los ángulos del servo y los umbrales en el código según el montaje físico.

Solución de problemas
----------------------
- El robot gira en la dirección equivocada: invierte uno de los pares de pines del motor (o cambia el cableado en el driver).
- Medidas erráticas del HC-SR04: asegúrate de que el sensor tenga alimentación estable y que el echo/trig estén conectados correctamente; añade un pequeño delay entre lecturas.
- El servo no mueve: comprueba alimentación y que la señal está en el pin definido (10).
- Motores no se mueven: verifica que el driver tenga alimentación, que los pines estén bien conectados y que GND esté común con el Arduino.

Explicación breve del código
----------------------------
El bucle principal mide distancias a izquierda, centro y derecha moviendo el servo. Según combinaciones de umbrales, el robot:
- Avanza si el frente está despejado.
- Corrige con giros suaves si los laterales están cerca.
- Retrocede o da una vuelta completa si está atascado.
También mantiene un historial de medidas frontales para detectar repetición (posible atasco) y desencadenar maniobras de escape.

Personalizaciones recomendadas
------------------------------
- Añadir un sensor de giro (IMU) o encoders para obtener navegación más fiable.
- Implementar un PID para control más suave de velocidad/giros.
- Añadir LEDs o señales sonoras para depuración.
