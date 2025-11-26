#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 imu(Wire);

const int pinoBuzzer = 3;
const int pinoBotao  = 6;
bool movimento = false;




 
float referenciaX = 0.0;
float referenciaY = 0.0;

unsigned long instanteParado = 0;
const unsigned long limiteTempo = 10000;   // 10s

static float zonaNeutra = 40.0;  // tolerância (deadzone)

// -----------------------------------------------------------
// Filtro suave baseado na sua lógica original (CORRIGIDO)
// -----------------------------------------------------------
float amortecer(float leitura, float &memoria) {
    static bool primeiraVez = true;

    if (primeiraVez) {
        memoria = leitura;
        primeiraVez = false;
        return memoria;
    }

    float diferenca = fabs(leitura - memoria);

    // Se mudança for grande → atualiza de vez (igual antes)
    if (diferenca >= zonaNeutra) {
        memoria = leitura;
    }
    else {
        // Dentro da zona neutra → NÃO suaviza, segue a leitura real
        memoria = leitura;
    }

    return memoria;
}

float ultimoSuaveX = 0.0;
float ultimoSuaveY = 0.0;

// -----------------------------------------------------------

void setup() {
    Serial.begin(9600);
    Wire.begin();

    imu.begin();
    imu.calcGyroOffsets(true);

    pinMode(pinoBuzzer, OUTPUT);
    pinMode(pinoBotao, INPUT_PULLUP);

    digitalWrite(pinoBuzzer, LOW);

    imu.update();
    referenciaX = imu.getAngleX();
    referenciaY = imu.getAngleY();

    instanteParado = millis();

    Serial.println("Sistema iniciado!");
}

void loop() {
    imu.update();

    // Leitura filtrada
    float x = amortecer(imu.getAngleX(), ultimoSuaveX);
    float y = amortecer(imu.getAngleY(), ultimoSuaveY);

    // ---------------------------------------------
    // Botão → redefinir base
    // ---------------------------------------------
    if (!digitalRead(pinoBotao)) {
        referenciaX = x;
        referenciaY = y;
        instanteParado = millis();  // reset total
        digitalWrite(pinoBuzzer, LOW);

        Serial.println("Base redefinida!");
        delay(3000);
    }

    // ---------------------------------------------
    // Detectar se está fora da zona permitida
    // ---------------------------------------------
    bool foraDaPosicao = false;

    if (fabsf(x - referenciaX) >= zonaNeutra) foraDaPosicao = true;
    if (fabsf(y - referenciaY) >= zonaNeutra) foraDaPosicao = true;

    // Atualiza variável movimento
    movimento = foraDaPosicao;

    // ---------------------------------------------
    // Se está fora → tempo corre
    // Se está dentro → tempo reseta
    // ---------------------------------------------
    if (movimento) {
        Serial.println("Fora da posicao!");
        // NÃO mexe no instanteParado
    } 
    else {
        instanteParado = millis();  // voltou para a posição segura
        noTone(pinoBuzzer);
        Serial.println("Dentro da posicao!");
    }

    // ---------------------------------------------
    // Se passou muito tempo fora → alerta contínuo
    // ---------------------------------------------
    if (movimento && (millis() - instanteParado >= limiteTempo)) {
        tone(pinoBuzzer, 262);   // toca infinito até voltar
    }

    // ---------------------------------------------
    // Debug
    // ---------------------------------------------
    Serial.print("X: "); Serial.print(x);
    Serial.print(" | Y: "); Serial.print(y);
    Serial.print(" | RefX: "); Serial.print(referenciaX);
    Serial.print(" | RefY: "); Serial.print(referenciaY);
    Serial.print(" | Tempo fora: ");
    Serial.println((millis() - instanteParado) / 1000);
    delay(500);
}