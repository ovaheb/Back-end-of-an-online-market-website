#ifndef __OFFER_H__
#define __OFFER_H__

#include "libraries.hpp"

class Offer
{
public:
    Offer(int id, float price, int productNumber, int amount)
        : offerId(id), offerAmount(amount), offerUnitPrice(price), productId(productNumber)
    {
        vector<DiscountCode> temp;
        discountCodes = temp;
    };
    int getProductId() { return productId; }
    int getOfferId() { return offerId; }
    int getOfferAmount() { return offerAmount; }
    float getOfferUnitPrice() { return offerUnitPrice; }
    vector<DiscountCode> getDiscountCodes() { return discountCodes; }
    float useDiscountCode(int);
    void editOffer(float, int);
    void addDiscount(DiscountCode);
private:
    int productId;
    float offerUnitPrice;
    int offerAmount;
    int offerId;
    vector<DiscountCode> discountCodes;
};
#endif