#include "APIAcess.h"

using namespace SAeD;

bool APIAcess::pubNewItem(area area)
{
    JsonDocument doc;

    doc["type"] = area.name;
}