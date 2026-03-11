#include <Adafruit_QMC5883P.h>  // Importa lib para módulo bussola
#include <Adafruit_Sensor.h>    // Importa lib genérica para sensores em geral
#include <micro_ros_arduino.h>  // Importa lib para Micro-ROS
#include <Wire.h>               // Importa lib para comunicação UART e I2C

// Defines
#define I2C_Luna_SDA 17         // Define os pinos I2C do TF-Luna
#define I2C_Luna_SCL 16         // Define os pinos I2C do TF-Luna
#define I2C_Luna_ADDRESS 0x10   // Define o endereço I2C do TF-Luna
#define I2C_Luna_COMMAND 0x00   // Define o endereço de comando I2C do TF-Luna
#define I2C_Luna_DATA_LENGTH 9  // Define o tamanho máximo dos dados I2C do TF-Luna

// Constantes
unsigned char buf[] = { 0x5A, 0x05, 0x00, 0x01, 0x60, };  // Comando inicial para "acordar" o TF-Luna
Adafruit_QMC5883P qmc;                                    // Importa objeto da bussola atráves da lib

// Declaração de funções
void configurarBussola();
void testarLuna();
void coletarDados();

// Ações iniciais
void setup() {
  Serial.begin(115200);                        // Configura e inicia a comunicação serial
  configurarBussola();                         // Configura a bussola
  testarLuna();                                // Configura o TF-Luna
  Serial.println("Tudo pronto para iniciar!"); // Menssagem inicial meramente estética
}

// Rotina de repetição
void loop() {
  coletarDados();
}

// Funções

// Está função configura o modulo de bussola, foi retirada
// do exemplo documentado do módulo da Adafruit
void configurarBussola() {
  if (!qmc.begin()) {
    Serial.println("Erro, bussola não identificada!");
  }

  qmc.setMode(QMC5883P_MODE_NORMAL);

  qmc5883p_mode_t currentMode = qmc.getMode();
  Serial.print("Mode: ");
  switch (currentMode) {
  case QMC5883P_MODE_SUSPEND:
    Serial.println("Suspend");
    break;
  case QMC5883P_MODE_NORMAL:
    Serial.println("Normal");
    break;
  case QMC5883P_MODE_SINGLE:
    Serial.println("Single");
    break;
  case QMC5883P_MODE_CONTINUOUS:
    Serial.println("Continuous");
    break;
  default:
    Serial.println("Unknown");
    break;
  }

  // Set ODR (Output Data Rate) to 50Hz
  qmc.setODR(QMC5883P_ODR_50HZ);
  qmc5883p_odr_t currentODR = qmc.getODR();
  Serial.print("ODR (Output Data Rate): ");
  switch (currentODR) {
  case QMC5883P_ODR_10HZ:
    Serial.println("10Hz");
    break;
  case QMC5883P_ODR_50HZ:
    Serial.println("50Hz");
    break;
  case QMC5883P_ODR_100HZ:
    Serial.println("100Hz");
    break;
  case QMC5883P_ODR_200HZ:
    Serial.println("200Hz");
    break;
  default:
    Serial.println("Unknown");
    break;
  }

  // Set OSR (Over Sample Ratio) to 4
  qmc.setOSR(QMC5883P_OSR_4);
  qmc5883p_osr_t currentOSR = qmc.getOSR();
  Serial.print("OSR (Over Sample Ratio): ");
  switch (currentOSR) {
  case QMC5883P_OSR_8:
    Serial.println("8");
    break;
  case QMC5883P_OSR_4:
    Serial.println("4");
    break;
  case QMC5883P_OSR_2:
    Serial.println("2");
    break;
  case QMC5883P_OSR_1:
    Serial.println("1");
    break;
  default:
    Serial.println("Unknown");
    break;
  }

  // Set DSR (Downsample Ratio) to 2
  qmc.setDSR(QMC5883P_DSR_2);
  qmc5883p_dsr_t currentDSR = qmc.getDSR();
  Serial.print("DSR (Downsample Ratio): ");
  switch (currentDSR) {
  case QMC5883P_DSR_1:
    Serial.println("1");
    break;
  case QMC5883P_DSR_2:
    Serial.println("2");
    break;
  case QMC5883P_DSR_4:
    Serial.println("4");
    break;
  case QMC5883P_DSR_8:
    Serial.println("8");
    break;
  default:
    Serial.println("Unknown");
    break;
  }

  // Set Range to 8G
  qmc.setRange(QMC5883P_RANGE_8G);
  qmc5883p_range_t currentRange = qmc.getRange();
  Serial.print("Range: ");
  switch (currentRange) {
  case QMC5883P_RANGE_30G:
    Serial.println("±30G");
    break;
  case QMC5883P_RANGE_12G:
    Serial.println("±12G");
    break;
  case QMC5883P_RANGE_8G:
    Serial.println("±8G");
    break;
  case QMC5883P_RANGE_2G:
    Serial.println("±2G");
    break;
  default:
    Serial.println("Unknown");
    break;
  }

  // Set SetReset mode to On
  qmc.setSetResetMode(QMC5883P_SETRESET_ON);
  qmc5883p_setreset_t currentSetReset = qmc.getSetResetMode();
  Serial.print("Set/Reset Mode: ");
  switch (currentSetReset) {
  case QMC5883P_SETRESET_ON:
    Serial.println("Set and Reset On");
    break;
  case QMC5883P_SETRESET_SETONLY:
    Serial.println("Set Only On");
    break;
  case QMC5883P_SETRESET_OFF:
    Serial.println("Set and Reset Off");
    break;
  default:
    Serial.println("Unknown");
    break;
  }
}

void testarLuna() {
  if (!Wire.begin(I2C_Luna_SDA, I2C_Luna_SCL)) {
    Serial.println("Erro, LiDAR não identificado!");
  }
}

void coletarDados() {

  // Coleta dados diretos do TF-Luna
  Wire.beginTransmission(I2C_Luna_ADDRESS);
  Wire.write(buf, 5);
  Wire.endTransmission();
  Wire.requestFrom(I2C_Luna_ADDRESS, I2C_Luna_DATA_LENGTH);
  //------------------------------------------------------//

  // Prepara variáveis para receber dados coletados
  uint8_t data[I2C_Luna_DATA_LENGTH] = { 0 };

  uint16_t distance = 0;
  uint16_t strength = 0;
  uint16_t temperature = 0;

  int checksum = 0;
  int index = 0;

  // Faz os calculos com os dados brutos e os armazena nas variáveis
  // anteriormente criadas
  while(Wire.available() > 0 && index < I2C_Luna_DATA_LENGTH) {
    data[index++] = Wire.read();
  }

  if (index == I2C_Luna_DATA_LENGTH) {
    distance = data[2] + data[3] * 256;
    strength = data[4] + data[5] * 256;
    temperature = data[6] + data[7] * 256;
  }

  Serial.printf("Distancia medida: %f cm\n", distance);
  
  if (qmc.isDataReady()) {
    int16_t x, y, z;
    qmc.getRawMagnetic(&x, &y, &z);
    
    Serial.printf("X: %f, Y: %f, Z: %f\n", x, y, z);
  }
}