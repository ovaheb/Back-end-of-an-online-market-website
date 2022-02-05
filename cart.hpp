#ifndef __CART_H__
#define __CART_H__

#include "libraries.hpp"
#include "offer.hpp"

class Cart
{
public:
    Cart()
    {
        vector<Offer*> temp;
        vector<int> temp2;
        vector<float> temp3;
        offers = temp;
        quantities = temp2;
        percentage = temp3;
    };
    void addItem(Offer*, int, float);
    int getCartSize() { return offers.size(); }
    vector<Offer*> getOffers() { return offers; }
    vector<int> getQuantities() { return quantities; }
    vector<float> getPercentage() { return percentage; }
private:
    vector<Offer*> offers;
    vector<int> quantities;
    vector<float> percentage;
};

#endif