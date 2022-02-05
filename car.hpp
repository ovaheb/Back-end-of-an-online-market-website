#ifndef __CAR_H__
#define __CAR_H__

#include "libraries.hpp"
#include "product.hpp"

class Car : public Product
{
public:
    Car(vector<string> details, int id)
        : Product(id, details[0], CAR_INDICATOR)
    {
        weight = stoi(details[1]);
        sitting = stoi(details[2]);
        numOfCylinders = stoi(details[3]);
        engineCapacity = stoi(details[4]);
        if (details[5] == "0")
            rearSensor = false;
        else
            rearSensor = true;
    }
    virtual vector<string> productDetail();
private:
    int weight;
    int sitting;
    int numOfCylinders;
    int engineCapacity;
    bool rearSensor;
};

#endif