#include "tv.hpp"
vector<string> Tv::productDetail()
{
    vector<string> result;
    result.push_back(productName);
    result.push_back(to_string(screenSize));
    result.push_back(screenType);
    result.push_back(resolution);
    result.push_back(to_string(threeD));
    result.push_back(to_string(hdr));
    return result;
}