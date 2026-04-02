#include <Arduino.h>
#include <Adafruit_QMC5883P.h>
#include <Adafruit_Sensor.h>

class Bussola
{
public:
    Bussola() {
        configureCompass();
    }

    bool testCompass();
    void collectCompassData();

private:
    Adafruit_QMC5883P compass;

    void configureCompass()
    {
        if (!testCompass())
        {
            Serial.println("Error, compass not found!");
        }

        compass.setMode(QMC5883P_MODE_NORMAL);

        compass.setODR(QMC5883P_ODR_50HZ);
        compass.setOSR(QMC5883P_OSR_4);
        compass.setDSR(QMC5883P_DSR_2);
        compass.setRange(QMC5883P_RANGE_8G);
        compass.setSetResetMode(QMC5883P_SETRESET_ON);
    }
};

void Bussola::collectCompassData()
{
    if (compass.isDataReady())
    {
        int16_t x, y, z;
        compass.getRawMagnetic(&x, &y, &z);

        Serial.printf("X: %f, Y: %f, Z: %f\n", (float)x, (float)y, (float)z);
    }
}

bool Bussola::testCompass()
{
    if (!compass.begin())
    {
        return false;
    }
    return true;
}