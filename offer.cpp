#include "offer.hpp"

void Offer::editOffer(float newPrice, int newAmount)
{
    offerUnitPrice = newPrice;
    offerAmount = newAmount;
}
void Offer::addDiscount(DiscountCode newDiscountCode)
{
    discountCodes.push_back(newDiscountCode);
}
float Offer::useDiscountCode(int index)
{
    float result = discountCodes[index].percentage;
    discountCodes.erase(discountCodes.begin() + index);
    return result;
}