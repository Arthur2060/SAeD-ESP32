#include "Claw.h"

using namespace SAeD;

void Claw::idle()
{
    Serial.println("0");
}
void Claw::get()
{
    Serial.println("1");
}
void Claw::put()
{
    Serial.println("2");
}