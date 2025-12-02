#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

// CONFIGURAÇÕES
const int pinoBuzzer = 3;
const int pinoBotao  = 6;

float referenciaX = 0.0;
float referenciaY = 0.0;

float zonaNeutra = 35.0;  // zona em graus permitida

unsigned long instanteParado = 0;
const unsigned long limiteTempo = 60000; // 60s

void setup() {
  Serial.begin(9600);
  Wire.begin();

  byte status = mpu.begin();
  Serial.print("Status inicialização: ");
  Serial.println(status); // deve ser 0

  delay(1000);

  Serial.println("Calibrando...");
  mpu.calcGyroOffsets();
  Serial.println("Pronto!");

  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);

  delay(500);
  mpu.update();

  // define a primeira referência
  referenciaX = mpu.getAngleX();
  referenciaY = mpu.getAngleY();

  instanteParado = millis();

  Serial.println("Sistema iniciado!");
}

void loop() {
  mpu.update();

  float x = mpu.getAngleX();
  float y = mpu.getAngleY();

  // ==============================
  // REDEFINIR BASE PELO BOTÃO
  // ==============================
  if (!digitalRead(pinoBotao)) { // botão pressionado
    referenciaX = x;
    referenciaY = y;
    instanteParado = millis();
    noTone(pinoBuzzer);

    Serial.print(">> Base redefinida! X=");
    Serial.print(x);
    Serial.print("  Y=");
    Serial.println(y);

    delay(3000);
  }

  // ==============================
  // VERIFICA SE ESTÁ FORA DA POSIÇÃO
  // ==============================
  bool foraDaPosicao = false;

  if (fabs(x - referenciaX) >= zonaNeutra) foraDaPosicao = true;
  if (fabs(y - referenciaY) >= zonaNeutra) foraDaPosicao = true;

  // ==============================
  // LÓGICA DE ALERTA
  // ==============================
  if (!foraDaPosicao) {
    instanteParado = millis();
    noTone(pinoBuzzer);
    Serial.println("Dentro da posição!");
  } else {
    Serial.println("Fora da posição!");

    if (millis() - instanteParado >= limiteTempo) {
      tone(pinoBuzzer, 262);
    }
  }

  // ==============================
  // DEBUG
  // ==============================
  Serial.print("X: "); Serial.print(x);
  Serial.print(" | Y: "); Serial.print(y);
  Serial.print(" | RefX: "); Serial.print(referenciaX);
  Serial.print(" | RefY: "); Serial.print(referenciaY);
  Serial.print(" | Tempo fora: ");
  Serial.println((millis() - instanteParado) / 1000);

  delay(50);
}
