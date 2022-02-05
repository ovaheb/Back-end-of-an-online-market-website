#include "buyer.hpp"


void Buyer::addTransaction(float value)
{
    transactions.push_back(calculateWallet() + value);
}
float Buyer::calculateWallet()
{
    return transactions.back();
}
void Buyer::addOrder(Cart* newOrder)
{
    orders.push_back(newOrder);
}