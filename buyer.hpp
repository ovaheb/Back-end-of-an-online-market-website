#ifndef __BUYER_H__
#define __BUYER_H__

#include "libraries.hpp"
#include "user.hpp"
#include "cart.hpp"

class Buyer : public User
{
public:
    Buyer(int id, string name, string pass, string mail)
        : User(id, name, pass, mail, BUYER_TYPE)
    {
        vector<float> temp;
        transactions = temp;
        transactions.push_back(0);
        vector<Cart*> temp2;
        orders = temp2;
    }
    vector<Cart*> getOrders() { return orders; }
    void addTransaction(float value);
    void addOrder(Cart*);
    float calculateWallet();
    vector<float> getWallet() { return transactions; }
private:
    vector<float> transactions;
    vector<Cart*> orders; 
};

#endif