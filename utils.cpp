#include "utils.h"
#include <math.h>

static double ToRadian(double degrees) 
{
    const double PI = 3.14159265358979323846;
    return (degrees * PI) / 180.0;
}

double Distance(double lat1, double lng1, double lat2, double lng2) 
{
    double earthRadius = 6371.0; // KM

    double dLat = ToRadian(lat2 - lat1);
    double dLng = ToRadian(lng2 - lng1);

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) + 
         std::cos(ToRadian(lat1)) * std::cos(ToRadian(lat2)) * 
         std::sin(dLng / 2) * std::sin(dLng/2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return earthRadius * c;
}
