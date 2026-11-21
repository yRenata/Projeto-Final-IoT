
#include "RoboCore_Vespa.h"
#include <BluetoothSerial.h>

#define LED_PIN 15

BluetoothSerial SerialBT;
VespaMotors motores;

// ---------------- Configurações ----------------
const int VELOCIDADE = 80;

const unsigned long MS_POR_GRAU = 8;

const char CHAR_SOLTAR = '0';

const unsigned long TEMPO_SEM_REPETICAO = 250;

const int NUM_SERVOS = 4;
VespaServo servos[NUM_SERVOS];
const uint8_t PINOS[NUM_SERVOS] = {VESPA_SERVO_S1, VESPA_SERVO_S2,
                                   VESPA_SERVO_S3, VESPA_SERVO_S4};

const int ANG_MIN[NUM_SERVOS]     = {  0,   0,   0,   0};
const int ANG_MAX[NUM_SERVOS]     = {180, 180, 180, 180};
const int ANG_INICIAL[NUM_SERVOS] = { 90,  90,  90,  90};

// ---------------- Estado ----------------
int angulo[NUM_SERVOS];
int direcao[NUM_SERVOS];        
bool indoParaInicial = false; 
unsigned long ultimoPasso = 0;
unsigned long ultimoComandoServo = 0;

bool appEnviaSoltar = false;

// ------------------------------------------------
void mostrarAngulos() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    Serial.print("S");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(angulo[i]);
    Serial.print(i < NUM_SERVOS - 1 ? " | " : "\n");
  }
}

bool pararServos() {
  bool estavaMexendo = false;
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (direcao[i] != 0) estavaMexendo = true;
    direcao[i] = 0;
  }
  indoParaInicial = false;
  if (estavaMexendo) mostrarAngulos();
  return estavaMexendo;
}

void comandoSoltar() {
  bool estavaMexendo = pararServos();
  bool logoAposServo = ultimoComandoServo != 0 &&
                       millis() - ultimoComandoServo < 3000;
  if (estavaMexendo || logoAposServo) appEnviaSoltar = true;
}

void iniciarServo(int indice, int dir) {
  indoParaInicial = false;
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (i != indice) direcao[i] = 0;
  }
  direcao[indice] = dir;
  ultimoComandoServo = millis();
}

void atualizarServos() {
  if (!appEnviaSoltar && !indoParaInicial &&
      millis() - ultimoComandoServo > TEMPO_SEM_REPETICAO) {
    pararServos();
  }

  if (millis() - ultimoPasso < MS_POR_GRAU) return;
  ultimoPasso = millis();

  bool algumMexendo = false;

  for (int i = 0; i < NUM_SERVOS; i++) {
    int dir = direcao[i];

    if (indoParaInicial) {
      if (angulo[i] < ANG_INICIAL[i])      dir = +1;
      else if (angulo[i] > ANG_INICIAL[i]) dir = -1;
      else                                 dir = 0;
    }

    if (dir == 0) continue;

    int novo = angulo[i] + dir;
    if (novo < ANG_MIN[i] || novo > ANG_MAX[i]) {
      direcao[i] = 0;
      continue;
    }

    angulo[i] = novo;
    servos[i].write(angulo[i]);
    algumMexendo = true;
  }

  if (indoParaInicial && !algumMexendo) {
    indoParaInicial = false;
    mostrarAngulos();
  }
}

void tratarComando(char c) {
  switch (c) {
    // ---- Movimento ----
    case 'F': motores.forward(VELOCIDADE);           break;
    case 'B': motores.backward(VELOCIDADE);          break;
    case 'L': motores.turn(-VELOCIDADE, VELOCIDADE); break;
    case 'R': motores.turn(VELOCIDADE, -VELOCIDADE); break;
    case 'S': motores.stop(); comandoSoltar(); break;

    // ---- Servos (segure o botão) ----
    case '1': iniciarServo(0, +1); break;
    case '2': iniciarServo(0, -1); break;
    case '3': iniciarServo(1, +1); break;
    case '4': iniciarServo(1, -1); break;
    case '5': iniciarServo(2, +1); break;
    case '6': iniciarServo(2, -1); break;
    case '7': iniciarServo(3, +1); break;
    case '8': iniciarServo(3, -1); break;

    case CHAR_SOLTAR: comandoSoltar(); break;

    case 'P':
      for (int i = 0; i < NUM_SERVOS; i++) direcao[i] = 0;
      indoParaInicial = true;
      break;

    default: return;
  }
}

// ------------------------------------------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  SerialBT.begin("VESPA_BT");

  motores.stop();

  for (int i = 0; i < NUM_SERVOS; i++) {
    angulo[i] = ANG_INICIAL[i];
    direcao[i] = 0;
    servos[i].attach(PINOS[i]);
    servos[i].write(angulo[i]);
    delay(200); 
  }

  digitalWrite(LED_PIN, HIGH);
  Serial.println("Robo pronto. Conecte em VESPA_BT.");
  mostrarAngulos();
}

void loop() {
  while (SerialBT.available()) {
    char c = (char)SerialBT.read();
    if (c != '\r' && c != '\n') {
      Serial.print("Recebido: ");
      Serial.println(c);
    }
    tratarComando(c);
  }

  atualizarServos();
}
