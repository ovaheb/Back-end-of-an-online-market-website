#ifndef __SELLER_H__
#define __SELLER_H__

#include "libraries.hpp"
#include "user.hpp"
#include "offer.hpp"

class Seller : public User
{
public:
    Seller(int id, string name, string pass, string mail)
        : User(id, name, pass, mail, SELLER_TYPE)
    {
        vector<float> temp;
        transactions = temp;
        transactions.push_back(0);
        vector<Offer*> temp2;
        offers = temp2;
    }
    void addTransaction(float value);
    float calculateWallet();
    vector<float> getWallet() { return transactions; }
    int searchProduct(int);
    void addOffer(Offer* newOffer);
    vector<Offer*> getOffers() { return offers; }
    int searchOffer(int);
    bool changeOffer(int, float, int);
    bool addDiscountCode(int, DiscountCode);
private:
    vector<float> transactions;
    vector<Offer*> offers;
};

#endif