#include <QMC5883LCompass.h>
#include <Adafruit_Sensor.h>

class Bussola
{
public:
    void begin() {
        configureCompass();
    }
    float collectCompassData();

private:
    QMC5883LCompass compass;

    void configureCompass()
    {
        compass.init();
    }
};

float Bussola::collectCompassData()
{
    compass.read();
    float x;
    x = compass.getAzimuth();
    return x;
}