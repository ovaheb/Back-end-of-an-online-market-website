#include "mobile.hpp"

vector<string> Mobile::productDetail()
{
    vector<string> result;
    result.push_back(productName);
    result.push_back(to_string(weight));
    result.push_back(to_string(cpuFrequency));
    result.push_back(to_string(memory));
    result.push_back(to_string(ram));
    result.push_back(to_string(displaySize));
    result.push_back(to_string(cameraResolution));
    result.push_back(os);
    return result;
}