//Pinos de conexao do modulo
#include "Arduino.h"
#include "vector"

#define pinS0 18
#define pinS1 19
#define pinS2 21
#define pinS3 22
#define pinOut 23
#define btnCalibra 26

//Pinos dos LEDs
#define pinoLedVerm 14
#define pinoLedVerd 12
#define pinoLedAzul 27


unsigned int R = 0;
unsigned int G = 0;
unsigned int B = 0;

unsigned int Cor;

unsigned int MRvermelho = 0;
unsigned int MGvermelho = 0;
unsigned int MBvermelho = 0;
unsigned int mRvermelho = 0;
unsigned int mGvermelho = 0;
unsigned int mBvermelho = 0;

std::vector<uint> detectaCor();

bool modoCalibracao = false;
unsigned long tempoInicioCalib = 0;
const int tempoCalibracao = 5000; // 5 segundos

void setup()
{
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(btnCalibra, INPUT);
  pinMode(pinOut, INPUT);

  pinMode(pinoLedVerm, OUTPUT);
  pinMode(pinoLedVerd, OUTPUT);
  pinMode(pinoLedAzul, OUTPUT);

  Serial.begin(9600);
  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);

  delay(2000);
}

void loop()
{
  detectaCor();

 if (Serial.available())
  {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "c")
    {
      Serial.println("Iniciando calibracao...");
      modoCalibracao = true;
      tempoInicioCalib = millis();

      // Zerar valores
      MRvermelho = 0;
      MGvermelho = 0;
      MBvermelho = 0;

      mRvermelho = 999999;
      mGvermelho = 999999;
      mBvermelho = 999999;
    }
  }

  if (modoCalibracao)
  {

    // Atualiza máximos
    if (R > MRvermelho) MRvermelho = R;
    if (G > MGvermelho) MGvermelho = G;
    if (B > MBvermelho) MBvermelho = B;

    // Atualiza mínimos
    if (R < mRvermelho) mRvermelho = R;
    if (G < mGvermelho) mGvermelho = G;
    if (B < mBvermelho) mBvermelho = B;

    // Tempo acabou?
    if (millis() - tempoInicioCalib >= tempoCalibracao)
    {
      modoCalibracao = false;
      Serial.println("Calibracao finalizada!");

      Serial.print("R: "); Serial.print(mRvermelho); Serial.print(" - "); Serial.println(MRvermelho);
      Serial.print("G: "); Serial.print(mGvermelho); Serial.print(" - "); Serial.println(MGvermelho);
      Serial.print("B: "); Serial.print(mBvermelho); Serial.print(" - "); Serial.println(MBvermelho);
    }

    return; 
  }
  
  //Mostra valores no serial monitor
  Serial.print("Vermelho :");
  Serial.print(R);
  
  Serial.print(" Verde : ");
  Serial.print(G);

  Serial.print(" Azul : ");
  Serial.print(B);
  Serial.println();

  //Verifica se a cor vermelha foi detectada
 if(( R < MRvermelho && R > mRvermelho) && 
     ( G < MGvermelho && G > mGvermelho) &&
     ( B < MBvermelho && B > mBvermelho))
  {
    Serial.println("Vermelho");
    digitalWrite(pinoLedVerm, HIGH); //Acende o led azul
    digitalWrite(pinoLedVerd, LOW);
    digitalWrite(pinoLedAzul, LOW);
  }

  Serial.println();

  //Delay para apagar os leds e reiniciar o processo
  delay(50);
  digitalWrite(pinoLedVerm, LOW);
  digitalWrite(pinoLedVerd, LOW);
  digitalWrite(pinoLedAzul, LOW);
}

// *********** Função de leitura so sensor de cor ********************
std::vector<uint> detectaCor() {
  //Vermelho
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  R = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  
  //Verde
  digitalWrite(pinS2, HIGH);
  G = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  //Azul
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  B = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
 return {R, G, B};
}
