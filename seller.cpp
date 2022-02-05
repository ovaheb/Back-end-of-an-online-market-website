#include "seller.hpp"

void Seller::addTransaction(float value)
{
    transactions.push_back(calculateWallet() + value);
}
float Seller::calculateWallet()
{
    return transactions.back();
}
int Seller::searchProduct(int id)
{
    if (offers.size() == 0)
        return NOT_FOUND;
    for (int i = 0; i < offers.size(); i++)
        if (offers[i]->getProductId() == id)
            return i;
    return NOT_FOUND;
}
void Seller::addOffer(Offer* newOffer)
{
    offers.push_back(newOffer);
}
int Seller::searchOffer(int id)
{
    for (int i = 0; i < offers.size(); i++)
        if (offers[i]->getOfferId() == id)
            return i;
    return NOT_FOUND;
}
bool Seller::changeOffer(int id, float newPrice, int newAmount)
{
    int offerId = searchOffer(id);
    if (offerId == NOT_FOUND)
        return false;
    offers[offerId]->editOffer(newPrice, newAmount);
    return true;
    
}
bool Seller::addDiscountCode(int id, DiscountCode newDiscountCode)
{
    int offerId = searchOffer(id);
    if (offerId == NOT_FOUND)
        return false;
    offers[offerId]->addDiscount(newDiscountCode);
    return true;
    
}