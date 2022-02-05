#ifndef __TV_H__
#define __TV_H__

#include "libraries.hpp"
#include "product.hpp"

class Tv : public Product
{
public:
    Tv(vector<string> details, int id)
        : Product(id, details[0], TV_INDICATOR)
    {
        screenSize = stoi(details[1]);
        screenType = details[2];
        resolution = details[3];
        if (details[4] == "0")
            threeD = false;
        else
            threeD = true;
        if (details[5] == "0")
            hdr = false;
        else
            hdr = true;
    }
    virtual vector<string> productDetail();
private:
    int screenSize;
    string screenType;
    string resolution;
    bool threeD;
    bool hdr;
};

#endif