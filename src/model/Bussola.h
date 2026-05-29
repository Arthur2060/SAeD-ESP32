#include "Adafruit_QMC5883P.h"
#include "Adafruit_Sensor.h"

class Bussola
{
public:
    void begin()
    {
        configureCompass();
    }
    float collectCompassData();

private:
    Adafruit_QMC5883P compass;

    void configureCompass()
    {
        compass.begin();
    }
};

float Bussola::collectCompassData()
{
    int16_t x, y, z;
    compass.getRawMagnetic(&x, &y, &z);

    float heading = atan2(y, y);

    float declinationAngle = 0.22;
    heading += declinationAngle;

    if (heading < 0)
        heading += 2 * PI;

    if (heading > 2 * PI)
        heading -= 2 * PI;

    return heading * 180 / M_PI;
}