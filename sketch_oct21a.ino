#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3); 

#define TRIG_PIN 10
#define ECHO_PIN 11

#define BUZZER_PIN 8
#define LED_PIN 9

const int DISTANCIA_UMBRAL = 60;          
const unsigned long TIEMPO_PAUSA = 100; 

bool estadoAlarma = false;
unsigned long ultimoTiempoLectura = 0;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(9600); 
  BT.begin(9600);
  
  Serial.println("Sistema Ultrasónico con Bluetooth listo.");
  BT.println("Sistema Ultrasoico con Bluetooth listo.");
}

long obtenerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracion * 0.034 / 2; 
  return distancia;
}

void loop() {
  if (millis() - ultimoTiempoLectura < TIEMPO_PAUSA && estadoAlarma == false) {
    return;
  }

  long cm = obtenerDistancia();

  if (cm > 0 && cm <= DISTANCIA_UMBRAL) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    if (estadoAlarma == false) {
      String mensaje = "¡Objeto detectado a " + String(cm) + " cm! Alarma activada.";
      
      Serial.println(mensaje);
      BT.println(mensaje); 
      
      estadoAlarma = true;
    }
  } 
  else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    if (estadoAlarma == true) {
      String mensaje = "Objeto fuera de rango. Iniciando pausa de seguridad...";
      
      Serial.println(mensaje);
      BT.println(mensaje); 
      
      estadoAlarma = false;
      ultimoTiempoLectura = millis();
    }
  }

  delay(60);
}}