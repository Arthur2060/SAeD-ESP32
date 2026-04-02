#include <Arduino.h>
#include <Wire.h>

class TFLuna
{
public:
    const int LiDAR_DATA_LENGTH = 9;

    TFLuna(int SCL, int SDA)
    {
        Wire.begin(SDA, SCL);
    }

    TFLuna()
    {
        Serial2.begin(9600, SERIAL_8N1, 16, 17);
    }

    float collectDataUART();
    float collectDataI2C();
};

float TFLuna::collectDataUART()
{
    uint8_t data[LiDAR_DATA_LENGTH] = {0};

    uint16_t distance = 0;
    uint16_t strength = 0;
    uint16_t temperature = 0;

    int checksum = 0;
    int index = 0;

    while (Serial2.available() && index < LiDAR_DATA_LENGTH)
    {
        data[index++] = Serial2.read();
    }

    if (index == LiDAR_DATA_LENGTH)
    {
        distance = data[2] + data[3] * 256;
        strength = data[4] + data[5] * 256;
        temperature = data[6] + data[7] * 256;
    }

    Serial.printf("Distance: %f cm\n", (float)distance);

    return distance;
}

float TFLuna::collectDataI2C()
{
    uint8_t data[LiDAR_DATA_LENGTH] = {0};

    uint16_t distance = 0;
    uint16_t strength = 0;
    uint16_t temperature = 0;

    int checksum = 0;
    int index = 0;

    while (Wire.available() && index < LiDAR_DATA_LENGTH)
    {
        data[index++] = Wire.read();
    }

    if (index == LiDAR_DATA_LENGTH)
    {
        distance = data[2] + data[3] * 256;
        strength = data[4] + data[5] * 256;
        temperature = data[6] + data[7] * 256;
    }

    Serial.printf("Distance: %f cm\n", (float)distance);
    
    return distance;
}