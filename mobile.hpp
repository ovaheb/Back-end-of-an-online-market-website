#ifndef __MOBILE_H__
#define __MOBILE_H__

#include "libraries.hpp"
#include "product.hpp"

class Mobile : public Product
{
public:
    Mobile(vector<string> details, int id)
        : Product(id, details[0], MOBILE_INDICATOR)
    {
        weight = stoi(details[1]);
        cpuFrequency = stod(details[2]);
        memory = stoi(details[3]);
        ram = stoi(details[4]);
        displaySize = stod(details[5]);
        cameraResolution = stoi(details[6]);
        os = details[7];
    }
    virtual vector<string> productDetail();
private:
    int weight;
    double cpuFrequency;
    int memory;
    int ram;
    double displaySize;
    int cameraResolution;
    string os;
};

#endif