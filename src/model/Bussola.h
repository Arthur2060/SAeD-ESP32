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

    float heading = atan2( y, x ) * 180.0 / PI;
	return (int)heading % 360;
}