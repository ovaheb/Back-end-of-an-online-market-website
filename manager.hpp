#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "libraries.hpp"
#include "product.hpp"
#include "user.hpp"
#include "car.hpp"
#include "tv.hpp"
#include "mobile.hpp"
#include "admin.hpp"
#include "seller.hpp"
#include "buyer.hpp"
#include "offer.cpp"
#include "cart.cpp"

class notFound{};
class permissionDenied{};
class badRequest{};

vector<string> parseInput(string input, char parsingCriteria);
string generateRandomString(size_t len);
size_t wordHasher(string word);

class Manager
{
public:
    Manager();
    string processCommand(string command);
    int searchUsername(string name);
    int searchEmail(string email);
    vector<string> productDetail(int);
    string processProductDetail(int);
    string processCompare(int, int);
    vector<string> getCsvTitles(int);
    string processImportProduct(string, string);
    string processSignup(vector<string>);
    string processComments(vector<string>);
    string processComment(vector<string>);
    string processWallet(vector<string>);
    string processOffer(vector<string>);
    string processMyOffers();
    string processOffers(vector<string>);
    string processChangeOffer(vector<string>);
    string processGenerateDiscountCode(vector<string>);
    string processOffersOnProduct(vector<string>);
    float processDiscountCode(string, int);
    string processAddToCart(vector<string>);
    string processCart(vector<string>);
    string processOrders(vector<string>);
    string processSubmitCart(vector<string>);
    string processLogout();
    string processGetProducts();
    string processLogin(vector<string>);
    string processChargeWallet(vector<string>);
    string processGETCommand(vector<string>);
    string processPOSTCommand(vector<string>);
    string processOffers();
    int getSellerOfOffer(int);
    bool isValidCommand(string);
private:
    vector<Product*> products;
    vector<User*> users;
    vector<string> carOrder;
    vector<string> tvOrder;
    vector<string> mobileOrder;
    vector<Offer*> offers;
    User* onlineUser;
    Cart* pendingCart;
    int productIdCounter;
    int userIdCounter;
    int offerIdCounter;
};

#endif