// Pines del sensor de ultrasonido
const int EchoPin = 4;   // GPIO4 para el Echo
const int TriggerPin = 16; // GPIO16 para el Trigger

// Pines de control para los motores
const int input1 = 33, input2 = 25, input3 = 26, input4 = 27;
const int enableA = 14, enableB = 12;

// Variables de control
const int distanciaDeteccion = 30;  // Distancia límite de detección en cm

void setup() {
  Serial.begin(9600);   
  pinMode(TriggerPin, OUTPUT); pinMode(EchoPin, INPUT);

  // Configurar pines de los motores
  pinMode(enableA, OUTPUT); pinMode(enableB, OUTPUT);
  pinMode(input1, OUTPUT); pinMode(input2, OUTPUT);
  pinMode(input3, OUTPUT); pinMode(input4, OUTPUT);

  pararMotores();
}

void loop() {
  float distancia = medirDistanciaCM();
  Serial.print("Distancia detectada: "); Serial.print(distancia); Serial.println(" cm");

  if (distancia > 0 && distancia <= distanciaDeteccion) {
    pararMotores();  // Detenerse de inmediato al detectar un obstáculo
    delay(500);

    // Sigue girando sobre su propio eje hasta que no haya obstáculo
    while (medirDistanciaCM() <= distanciaDeteccion) {
      giroSobreEje();  
      delay(100);  // Pequeña pausa para suavizar el giro
    }

    // Una vez despejado el camino, avanzar
    avanzar(255);
  } else {
    avanzar(255);  // Avanza normalmente si no hay obstáculo
  }

  delay(100);
}

// Medir la distancia con el sensor de ultrasonido
float medirDistanciaCM() {
  digitalWrite(TriggerPin, LOW); delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH); delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  unsigned long duracion = pulseIn(EchoPin, HIGH, 30000);
  return (duracion == 0) ? -1 : (duracion * 0.0343) / 2;
}

// Avanzar con una velocidad dada
void avanzar(int velocidad) {
  Serial.println("Avanzando...");
  moverMotores(LOW, HIGH, LOW, HIGH, velocidad, velocidad);
}

// Giro sobre su propio eje
void giroSobreEje() {
  Serial.println("Girando sobre su propio eje...");
  moverMotores(LOW, HIGH, HIGH, LOW, 150, 150);  // Motores en direcciones opuestas
}

// Parar los motores
void pararMotores() {
  Serial.println("Deteniendo...");
  moverMotores(LOW, LOW, LOW, LOW, 0, 0);
}

// Controlar el movimiento de los motores
void moverMotores(int in1, int in2, int in3, int in4, int velA, int velB) {
  digitalWrite(input1, in1); digitalWrite(input2, in2);
  digitalWrite(input3, in3); digitalWrite(input4, in4);
  analogWrite(enableA, velA); analogWrite(enableB, velB);
}
