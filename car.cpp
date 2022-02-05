#include "car.hpp"

vector<string> Car::productDetail()
{
    vector<string> result;
    result.push_back(productName);
    result.push_back(to_string(weight));
    result.push_back(to_string(sitting));
    result.push_back(to_string(numOfCylinders));
    result.push_back(to_string(engineCapacity));
    result.push_back(to_string(rearSensor));
    return result;
}