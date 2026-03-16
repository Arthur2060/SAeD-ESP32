#include <Adafruit_QMC5883P.h>  
#include <Adafruit_Sensor.h>    
#include <Wire.h>               

#define LiDAR_DATA_LENGTH 9

Adafruit_QMC5883P compass;

bool testCompass();

void configureCompass();

void collectCompassData();
void collectLiDARData();

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  configureCompass();
  Serial.println("Everything good to start!");
}

void loop() {
  collectLiDARData();
  if (testCompass()) {
    collectCompassData();
  }
}

void configureCompass() {
  if (!testCompass()) {
    Serial.println("Error, compass not found!");
  }

  compass.setMode(QMC5883P_MODE_NORMAL);
  
  compass.setODR(QMC5883P_ODR_50HZ);
  compass.setOSR(QMC5883P_OSR_4);
  compass.setDSR(QMC5883P_DSR_2);
  compass.setRange(QMC5883P_RANGE_8G);
  compass.setSetResetMode(QMC5883P_SETRESET_ON);
}

bool testCompass() {
  if (!compass.begin()) {
    return false;
  }
  return true;
}

void collectLiDARData() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX2=16, TX2=17
  //------------------------------------------------------//

  // Prepares variables for LiDAR`s data
  uint8_t data[LiDAR_DATA_LENGTH] = { 0 };

  uint16_t distance = 0;
  uint16_t strength = 0;
  uint16_t temperature = 0;

  int checksum = 0;
  int index = 0;

  // Calculate the brute data from
  // LiDAR for good use
  while(Serial2.available() && index < LiDAR_DATA_LENGTH) {
    data[index++] = Serial2.read();
  }

  if (index == LiDAR_DATA_LENGTH) {
    distance = data[2] + data[3] * 256;
    strength = data[4] + data[5] * 256;
    temperature = data[6] + data[7] * 256;
  }

  Serial.printf("Distance: %f cm\n", distance);
}

void collectCompassData() {
  if (compass.isDataReady()) {
    int16_t x, y, z;
    compass.getRawMagnetic(&x, &y, &z);
    
    Serial.printf("X: %f, Y: %f, Z: %f\n", x, y, z);
  }
}