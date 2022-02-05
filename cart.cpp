#include "cart.hpp"

void Cart::addItem(Offer* chosenOffer, int quantity, float discount)
{
    offers.push_back(chosenOffer);
    quantities.push_back(quantity);
    percentage.push_back(discount);
}