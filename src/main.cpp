#include "Arduino.h"

//Pinos de conexao do modulo
#define pinS0 18
#define pinS1 19
#define pinS2 21
#define pinS3 22
#define pinOut 23

//Pinos do LED RGB
#define pinoLedVerm 12
#define pinoLedVerd 14
#define pinoLedAzul 27

unsigned int R = 0;
unsigned int G = 0;
unsigned int B = 0;

//  VERMELHO
unsigned int MRv=0, MGv=0, MBv=0;
unsigned int mRv=999999, mGv=999999, mBv=999999;

//  VERDE
unsigned int MRg=0, MGg=0, MBg=0;
unsigned int mRg=999999, mGg=999999, mBg=999999;

//  AZUL
unsigned int MRb=0, MGb=0, MBb=0;
unsigned int mRb=999999, mGb=999999, mBb=999999;

bool modoCalibracao = false;
String corCalibrando = "";

unsigned long tempoInicioCalib = 0;
const int tempoCalibracao = 5000;

// -------- FUNÇÕES --------
void detectaCor();
void setColor(bool r, bool g, bool b);
void detectarCores();

void setup()
{
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
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

  //  COMANDOS SERIAL
  if (Serial.available())
  {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "cr")
    {
      Serial.println("Calibrando VERMELHO...");
      corCalibrando = "vermelho";

      MRv=0; MGv=0; MBv=0;
      mRv=999999; mGv=999999; mBv=999999;

      modoCalibracao = true;
      tempoInicioCalib = millis();
    }

    else if (comando == "cg")
    {
      Serial.println("Calibrando VERDE...");
      corCalibrando = "verde";

      MRg=0; MGg=0; MBg=0;
      mRg=999999; mGg=999999; mBg=999999;

      modoCalibracao = true;
      tempoInicioCalib = millis();
    }

    else if (comando == "cb")
    {
      Serial.println("Calibrando AZUL...");
      corCalibrando = "azul";

      MRb=0; MGb=0; MBb=0;
      mRb=999999; mGb=999999; mBb=999999;

      modoCalibracao = true;
      tempoInicioCalib = millis();
    }
  }

  //  MODO CALIBRAÇÃO
  if (modoCalibracao)
  {
    if (corCalibrando == "vermelho")
    {
      if (R > MRv) MRv = R;
      if (G > MGv) MGv = G;
      if (B > MBv) MBv = B;

      if (R < mRv) mRv = R;
      if (G < mGv) mGv = G;
      if (B < mBv) mBv = B;
    }

    else if (corCalibrando == "verde")
    {
      if (R > MRg) MRg = R;
      if (G > MGg) MGg = G;
      if (B > MBg) MBg = B;

      if (R < mRg) mRg = R;
      if (G < mGg) mGg = G;
      if (B < mBg) mBg = B;
    }

    else if (corCalibrando == "azul")
    {
      if (R > MRb) MRb = R;
      if (G > MGb) MGb = G;
      if (B > MBb) MBb = B;

      if (R < mRb) mRb = R;
      if (G < mGb) mGb = G;
      if (B < mBb) mBb = B;
    }

    if (millis() - tempoInicioCalib >= tempoCalibracao)
    {
      modoCalibracao = false;
      Serial.println("Calibracao finalizada!");
    }

    return;
  }

  //  DETECÇÃO
  detectarCores();

  // DEBUG
  Serial.print("R: "); Serial.print(R);
  Serial.print(" G: "); Serial.print(G);
  Serial.print(" B: "); Serial.println(B);

  delay(200);
}

// -------- LEITURA SENSOR --------
void detectaCor()
{
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  R = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  digitalWrite(pinS2, HIGH);
  G = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  B = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
}

// -------- DETECÇÃO DE CORES --------
void detectarCores()
{
  //  Vermelho
  if ((R >= mRv && R <= MRv) &&
      (G >= mGv && G <= MGv) &&
      (B >= mBv && B <= MBv))
  {
    Serial.println("VERMELHO");
    setColor(1,0,0);
    return;
  }

  //  Verde
  if ((R >= mRg && R <= MRg) &&
      (G >= mGg && G <= MGg) &&
      (B >= mBg && B <= MBg))
  {
    Serial.println("VERDE");
    setColor(0,1,0);
    return;
  }

  //  Azul
  if ((R >= mRb && R <= MRb) &&
      (G >= mGb && G <= MGb) &&
      (B >= mBb && B <= MBb))
  {
    Serial.println("AZUL");
    setColor(0,0,1);
    return;
  }

  // Nenhuma cor
  setColor(0,0,0);
}

// -------- RGB --------
void setColor(bool r, bool g, bool b)
{
  digitalWrite(pinoLedVerm, r);
  digitalWrite(pinoLedVerd, g);
  digitalWrite(pinoLedAzul, b);
}
