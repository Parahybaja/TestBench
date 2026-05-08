#include <Arduino.h>
#include "HX711.h"

// Configurações de hardware
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
const int OP_PIN = 9;          // Botão de calibração
const int RESET_PIN = 8;       // Botão de reset manual
const float PESO_CALIBRACAO = 57000; // 57Kg em gramas

// Parâmetros de medição
const int NUM_LEITURAS = 1;         // Leituras por ciclo
const int TEMPO_LEITURA = 12;     // Intervalo entre leituras
const float LIMITE_DERIVA = 0.5;    // Limite de deriva em kg

HX711 scale;
float CALIBRATION_FACTOR = 12.0;
int contadorLeituras = 0;

// Variáveis de estado
bool buttonPressed = false;
bool resetRequested = false;
float weightHistory[NUM_LEITURAS];
byte historyIndex = 0;

void resetWeightHistory() {
  for(byte i = 0; i < NUM_LEITURAS; i++) {
    weightHistory[i] = 0;
  }
  historyIndex = 0;
}

void performTare() {
  scale.tare(NUM_LEITURAS);
  resetWeightHistory();
  Serial.println("Tara realizada");
}

void initializeScale() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  performTare();
  Serial.println("Sistema inicializado");
}

void handleReset() {
  Serial.println("\n=== RESET MANUAL ===");
  performTare();
  contadorLeituras = 0;
  Serial.println("Reset completo. Reiniciando medições...");
  Serial.println("--------------------------------------");
  delay(1000);
}

void calibrateScale() {
  Serial.println("\n=== CALIBRAÇÃO INICIADA ===");
  
  Serial.println("Remova TODOS os pesos");
  delay(5000);
  performTare();
  
  Serial.print("Coloque EXATAMENTE ");
  Serial.print(PESO_CALIBRACAO/1000);
  Serial.println(" kg");
  delay(10000);

  long rawValue = scale.read_average(NUM_LEITURAS);
  CALIBRATION_FACTOR = rawValue / PESO_CALIBRACAO;
  Serial.println(rawValue);
  scale.set_scale(CALIBRATION_FACTOR);
  
  float verified = scale.get_units(NUM_LEITURAS) / 1000.0;
  Serial.print("Fator calculado: ");
  Serial.print(CALIBRATION_FACTOR, 6);
  Serial.print(" | Peso verificado: ");
  Serial.print(verified, 3);
  Serial.println(" kg");
  
  Serial.println("=== CALIBRAÇÃO CONCLUÍDA ===");
  Serial.println("--------------------------------------");
  delay(2000);
}

void handleCalibration() {
  calibrateScale();
}

void checkButtons() {
  // Verifica botão de reset (acionamento momentâneo)
  if(digitalRead(RESET_PIN) == LOW) {
    delay(50); // Debounce
    if(digitalRead(RESET_PIN) == LOW) {
      resetRequested = true;
      while(digitalRead(RESET_PIN) == LOW); // Espera soltar
    }
  }

  // Verifica botão de calibração (pressione longo)
  static unsigned long pressStart = 0;
  if(digitalRead(OP_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStart = millis();
  }
  else if(digitalRead(OP_PIN) == HIGH && buttonPressed) {
    buttonPressed = false;
    if(millis() - pressStart > 2000) {
      handleCalibration();
    }
  }
}

void setup() {
  // Aumente o Baud Rate para não gargalar a transmissão de dados
  Serial.begin(115200); 
  
  pinMode(OP_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare();
  
  Serial.println("\nSistema Otimizado para Alta Velocidade");
}

void loop() {
  static unsigned long lastRead = 0;
  
  checkButtons();

  if(resetRequested) {
    handleReset();
    resetRequested = false;
    lastRead = millis(); // Reseta o timer de leitura
    return;
  }

  // Leituras periódicas
  if(millis() - lastRead >= TEMPO_LEITURA) {
    if(scale.wait_ready_timeout(500)) {
      float weight = scale.get_units(NUM_LEITURAS) / 1000.0; // kg
      
      // Armazena e exibe a leitura
      weightHistory[historyIndex % NUM_LEITURAS] = weight;
      historyIndex++;
      contadorLeituras++;
      unsigned long tempoAtual = millis(); // Captura o timestamp

      Serial.print(tempoAtual); // Imprime o tempo em ms
      Serial.print("ms | ");
      Serial.print("Leitura ");
      Serial.print(contadorLeituras);
      Serial.print(": ");
      Serial.print(weight, 3);
      Serial.print(" kg | ");
      Serial.print(weight * 9.80665, 1);
      Serial.println(" N");
    }
    lastRead = millis();
  }
}
