#include "manager.hpp"

vector<string> parseInput(string input, char parsingCriteria)
{
    vector<string> brokenLine;
    string word;
    istringstream s(input);
    while (getline(s, word, parsingCriteria))
        brokenLine.push_back(word);    
    return brokenLine;
}
Manager::Manager()
{
    vector<Product*> temp;
    products = temp;
    vector<User*> temp2;
    users = temp2;
    vector<string> temp3;
    tvOrder = temp3;
    carOrder = temp3;
    mobileOrder = temp3;
    users.push_back(new Admin());
    onlineUser = users[0];
    pendingCart = NULL;
    productIdCounter = STARTING_ID_NUMBER;
    userIdCounter = STARTING_ID_NUMBER;
    offerIdCounter = STARTING_ID_NUMBER;
}
int Manager::searchUsername(string name)
{
    int index = NOT_FOUND;
    for (int i = 0; i < users.size(); i++)
        if (users[i]->getUsername() == name)
            index = i;
    return index;
}
int Manager::searchEmail(string email)
{
    int index = NOT_FOUND;
    for (int i = 0; i < users.size(); i++)
        if (users[i]->getEmail() == email)
            index = i;
    return index;
}
vector<string> Manager::productDetail(int wantedProduct)
{
    if (wantedProduct > products.size())
                throw notFound();
    return products[wantedProduct]->productDetail();
}
string Manager::processProductDetail(int wantedProduct)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    string result;
    vector<string> productDetails = productDetail(wantedProduct);
    result += productDetails[0];
    result += NEXT_LINE;
    for (int i = 1; i < productDetails.size(); i++)
    {
        if (products[wantedProduct]->getProductType() == CAR_INDICATOR)
            result += carOrder[i];
        if (products[wantedProduct]->getProductType() == MOBILE_INDICATOR)
            result += mobileOrder[i];
        if (products[wantedProduct]->getProductType() == TV_INDICATOR)
            result += tvOrder[i];
        result += ": ";
        result += productDetails[i];
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processOffer(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != SELLER_TYPE)   
        throw permissionDenied();
    int id = stoi(commandParts[4]);
    if (id > products.size())
        throw badRequest();
    if (((Seller*)onlineUser)->searchProduct(id) != NOT_FOUND)
        throw badRequest();
    Offer* newOffer = new Offer(offerIdCounter, stof(commandParts[6]), id, stoi(commandParts[8]));
    offers.push_back(newOffer);
    offerIdCounter++;
    ((Seller*)onlineUser)->addOffer(newOffer);
    return OK;
}
vector<string> Manager::getCsvTitles(int productType)
{
    if (productType == CAR_INDICATOR) return carOrder;
    if (productType == MOBILE_INDICATOR) return mobileOrder;
    if (productType == TV_INDICATOR) return tvOrder;
}
string Manager::processCompare(int firstProduct, int secondProduct)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    string result;
    vector<string> firstProductDetails, secondProductDetails;
    if (products[firstProduct]->getProductType() != products[secondProduct]->getProductType())
        throw badRequest();
    firstProductDetails = productDetail(firstProduct);
    secondProductDetails = productDetail(secondProduct);
    vector<string> titles = getCsvTitles(products[firstProduct]->getProductType());
    result += firstProductDetails[0];
    result += DIVIDER;
    result += secondProductDetails[0];
    result += NEXT_LINE;
    for (int i = 1; i < firstProductDetails.size(); i++)
    {
        result += titles[i];
        result += ": ";
        result += firstProductDetails[i];
        result += DIVIDER;
        result += secondProductDetails[i];
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processComments(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    if (stoi(commandParts[4]) > products.size())
        throw notFound();
    string result;
    vector<Comment> productComments = products[stoi(commandParts[4]) - 1]->getComments();
    result += products[stoi(commandParts[4]) - 1]->getProductName();
    result += NEXT_LINE;
    for (int i = 0; i < productComments.size(); i++)
    {
        result += productComments[i].username;
        result += DIVIDER;
        result += productComments[i].text;
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processImportProduct(string filePath, string fileType)
{   
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != ADMIN_TYPE) 
        throw permissionDenied();
    ifstream csvFile;
    Product* tempProduct;
    csvFile.open(filePath, ios::in);
    if (!csvFile.is_open()) 
    {
        csvFile.close();
        throw notFound();
    }
    vector<string> brokenLine;
    string orderOfFeatures, line;
    getline(csvFile, orderOfFeatures);
    vector<string> brokenOrder = parseInput(orderOfFeatures, COMMA);
    while (getline(csvFile, line))
    {
        brokenLine = parseInput(line, COMMA);
        if (fileType == "mobile")
        {
            tempProduct = new Mobile(brokenLine, productIdCounter);
            mobileOrder = brokenOrder;
        }
        else if (fileType == "tv")
        {
            tempProduct = new Tv(brokenLine, productIdCounter);
            tvOrder = brokenOrder;
        }
        else if (fileType == "car")
        {
            tempProduct = new Car(brokenLine, productIdCounter);
            carOrder = brokenOrder;
        }
        else
            throw notFound();
        productIdCounter++;
        products.push_back(tempProduct);
    }
    return OK;
}
string Manager::processSignup(vector<string> commandParts)
{
    if (onlineUser != NULL)
        throw badRequest();
    User* tempUser;
    int commandSize = commandParts.size();
    if ((searchUsername(commandParts[6]) != NOT_FOUND) || (searchEmail(commandParts[4]) != NOT_FOUND))
        throw badRequest();
    if (commandSize == 11)
    {
        if (commandParts[9] != "type")
            throw notFound();
        if (commandParts[10] == "buyer")
            tempUser = new Buyer(userIdCounter, commandParts[6], commandParts[8], commandParts[4]);
        else if (commandParts[10] == "seller")
            tempUser = new Seller(userIdCounter, commandParts[6], commandParts[8], commandParts[4]);
    }
    else
        tempUser = new Buyer(userIdCounter, commandParts[6], commandParts[8], commandParts[4]);
    userIdCounter++;
    users.push_back(tempUser);
    onlineUser = users[userIdCounter - 1];
    return OK;
}
string Manager::processComment(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    int commandSize = commandParts.size();
    if (stoi(commandParts[4]) > products.size())
        throw notFound();
    Comment newComment;
    newComment.username = onlineUser->getUsername();
    for (int i = 6; i < commandSize; i++)
    {
        newComment.text += commandParts[i];
        if (i != (commandSize - 1))
            newComment.text += " ";
    }   
    products[stoi(commandParts[4]) - 1]->addComment(newComment);
    return OK;
}
string Manager::processWallet(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() == ADMIN_TYPE)
        throw permissionDenied();
    int indicator;
    vector<float> transactions;
    if (onlineUser->getUserType() == BUYER_TYPE)
        transactions = ((Buyer*)onlineUser)->getWallet();
    else if (onlineUser->getUserType() == SELLER_TYPE)
        transactions = ((Seller*)onlineUser)->getWallet();
    string wallet = "credit\n";
    if (stoi(commandParts[4]) > transactions.size())
        indicator = 0;
    else
        indicator = transactions.size() - stoi(commandParts[4]);
    for (int i = transactions.size() - 1; i >= indicator; i--)
    {
        wallet += to_string(transactions[i]);
        wallet += NEXT_LINE;
    }
    return wallet;
}
string Manager::processMyOffers()
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != SELLER_TYPE)
        throw permissionDenied();
    vector<Offer*> myOffers = ((Seller*)onlineUser)->getOffers();
    string result = WALLET_TITLE;
    for (int i = 0; i < myOffers.size(); i++)
    {
        result += to_string(myOffers[i]->getProductId());
        result += DIVIDER;
        result += to_string(myOffers[i]->getOfferId());
        result += DIVIDER;
        result += to_string(myOffers[i]->getOfferUnitPrice());
        result += DIVIDER;
        result += to_string(myOffers[i]->getOfferAmount());
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processOffers(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != ADMIN_TYPE)
        throw permissionDenied();
    string result = ADMIN_OFFERS_TITLE;
    int index;
    vector<Offer*> offersToPrint;
    if (commandParts[6] == "offerId")
        offersToPrint = offers;
    else if (commandParts[6] == "offerPrice")
    {
        offersToPrint = offers;
        for (int j = 0; j < offersToPrint.size(); j++)
        {
            for (int i = 0; i < offersToPrint.size() - 1; i++)
            {
                if (offersToPrint[i]->getOfferUnitPrice() > offersToPrint[i + 1]->getOfferUnitPrice())
                    iter_swap(offersToPrint.begin() + i, offersToPrint.begin() + (i + 1));
                if (offersToPrint[i]->getOfferUnitPrice() == offersToPrint[i + 1]->getOfferUnitPrice())
                    if (offersToPrint[i]->getOfferId() > offersToPrint[i + 1]->getOfferId())
                        iter_swap(offersToPrint.begin() + i, offersToPrint.begin() + (i + 1));
            }
        }
    }
    for (int i = 0; i < offersToPrint.size(); i++)
    {
        if (commandParts[4] == "ASCEND")
            index = i;
        else
            index = (offersToPrint.size() - 1) - i;
        result += to_string(offersToPrint[index]->getProductId());
        result += DIVIDER;
        result += to_string(offersToPrint[index]->getOfferId());
        result += DIVIDER;
        result += to_string(offersToPrint[index]->getOfferUnitPrice());
        result += DIVIDER;
        result += to_string(offersToPrint[index]->getOfferAmount());
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processChangeOffer(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != SELLER_TYPE)   
        throw permissionDenied();
    int id = stoi(commandParts[4]);
    if (id > offers.size())
        throw badRequest();
    offers[id - 1]->editOffer(stof(commandParts[6]), stoi(commandParts[8]));
    bool success = ((Seller*)onlineUser)->changeOffer(id, stof(commandParts[6]), stoi(commandParts[8]));
    if (success)
        return OK;
    throw badRequest();
}
string generateRandomString(size_t len)
{
    string randomString;
    for (size_t i = 0; i < len; ++i) {
        int randomChar = rand()%(26+26+10);
        if (randomChar < 26)
            randomString += ('a' + randomChar);
        else if (randomChar < 26+26)
            randomString += ('A' + randomChar - 26);
        else
            randomString += ('0' + randomChar - 26 - 26);
     }
    return randomString;
}
string Manager::processGenerateDiscountCode(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != SELLER_TYPE)   
        throw permissionDenied();
    string result;
    DiscountCode newDiscountCode;
    int id = stoi(commandParts[4]);
    if (id > offers.size())
        throw badRequest();
    if ((stoi(commandParts[6]) > 100) || (stoi(commandParts[6]) < 0))
        throw badRequest();
    if (stoi(commandParts[8]) <= 0)
        throw badRequest();
    for (int i = 0; i < stoi(commandParts[8]); i++)
    {
        newDiscountCode.code = generateRandomString(lenOfDiscountCode);
        newDiscountCode.percentage = stof(commandParts[6]);
        offers[id - 1]->addDiscount(newDiscountCode);
        bool success = ((Seller*)onlineUser)->addDiscountCode(id, newDiscountCode);
        if (!success)
            throw badRequest();
        result += newDiscountCode.code;
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processOffersOnProduct(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    string result = OFFERS_ON_PRODUCT_TITLE;
    int id = stoi(commandParts[4]);
    if (id > products.size())
        throw badRequest();
    string productName = products[id - 1]->getProductName();
    for (int i = 0; i < offers.size(); i++)
        if (offers[i]->getProductId() == id)
        {
            result += to_string(id);
            result += DIVIDER;
            result += productName;
            result += DIVIDER;
            result += to_string(offers[i]->getOfferId());
            result += DIVIDER;
            result += to_string(offers[i]->getOfferUnitPrice());
            result += DIVIDER;
            result += to_string(offers[i]->getOfferAmount());
            result += NEXT_LINE;
        }
    return result;
}
float Manager::processDiscountCode(string usedDiscountCode, int offerId)
{
    vector<DiscountCode> codes = offers[offerId]->getDiscountCodes();
    for (int i = 0; i < codes.size(); i++)
        if (codes[i].code == usedDiscountCode)
            return offers[i]->useDiscountCode(i);
    return NOT_FOUND;
}
int Manager::getSellerOfOffer(int id)
{
    for (int i = 0; i < users.size(); i++)
        if (users[i]->getUserType() == SELLER_TYPE)
            if(((Seller*)users[i])->searchOffer(id) != NOT_FOUND)
                return users[i]->getUserId();
    return NOT_FOUND;
}
string Manager::processCart(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    if (pendingCart == NULL)
        return EMPTY_CART;
    string result = CART_TITLE;
    for (int i = (pendingCart->getCartSize() - 1); i >= 0; i--)
    {
        result += to_string((pendingCart->getOffers())[i]->getProductId());
        result += DIVIDER;
        result += products[(pendingCart->getOffers())[i]->getProductId() - 1]->getProductName();
        result += DIVIDER;
        result += to_string((pendingCart->getOffers())[i]->getOfferId());
        result += DIVIDER;
        result += to_string(getSellerOfOffer((pendingCart->getOffers())[i]->getOfferId()));
        result += DIVIDER;
        result += to_string((pendingCart->getPercentage())[i] * (pendingCart->getOffers())[i]->getOfferUnitPrice());
        result += DIVIDER;
        result += to_string((pendingCart->getQuantities())[i]);
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processAddToCart(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)   
        throw permissionDenied();
    if (commandParts.size() == 9)
        if (commandParts[7] != "discountCode")
            throw badRequest();
    int offerId = stoi(commandParts[4]);
    float discountPercentage = 1;
    if (offerId > offers.size())
        throw badRequest();
    if (offers[offerId - 1]->getOfferAmount() < stoi(commandParts[6]))
        throw badRequest();
    if (commandParts.size() == 9)
        discountPercentage = ((HUNDRED - processDiscountCode(commandParts[8], offerId)) / HUNDRED);
    if (pendingCart == NULL)
        pendingCart = new Cart();
    pendingCart->addItem(offers[offerId - 1], stoi(commandParts[6]), discountPercentage);
    offers[offerId - 1]->editOffer(offers[offerId - 1]->getOfferUnitPrice(), offers[offerId - 1]->getOfferAmount() - stoi(commandParts[6]));
    ((Seller*)onlineUser)->changeOffer(offerId, offers[offerId - 1]->getOfferUnitPrice(), offers[offerId - 1]->getOfferAmount() - stoi(commandParts[6]));
    return OK;
}
bool Manager::isValidCommand(string command)
{
    if ((command == "login") || (command == "signup") || (command == "logout") || (command == "importProducts") || (command == "offers")
    || (command == "getProducts") || (command == "offer") || (command == "myOffers") || (command == "changeOffer") || (command == "wallet")
    || (command == "generateDiscountCode") || (command == "getProducts") || (command == "offersOnProduct") || (command == "productDetail")
    || (command == "comments") || (command == "addToCart") || (command == "cart") || (command == "submitCart") || (command == "orders")
    || (command == "chargeWallet") || (command == "comment") || (command == "compare") || (command == "offers"))
        return true;
    else
        return false;
}
string Manager::processOrders(vector<string> commandParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    int indicator;
    string result = ORDERS_TITLE;
    vector<Cart*> orders = ((Buyer*)onlineUser)->getOrders();
    if (orders.size() == 0)
        throw notFound();
    if (stoi(commandParts[4]) > orders.size())
        indicator = 0;
    else
        indicator = orders.size() - stoi(commandParts[4]);
    for (int j = orders.size() - 1; j >= indicator; j--)
    {  
        Cart* temp = orders[j];
        for (int i = (temp->getCartSize() - 1); i >= 0; i--)
        {
            result += to_string((temp->getOffers())[i]->getProductId());
            result += DIVIDER;
            result += products[(temp->getOffers())[i]->getProductId() - 1]->getProductName();
            result += DIVIDER;
            result += to_string((temp->getOffers())[i]->getOfferId());
            result += DIVIDER;
            result += to_string(getSellerOfOffer((temp->getOffers())[i]->getOfferId()));
            result += DIVIDER;
            result += to_string((temp->getPercentage())[i] * (temp->getOffers())[i]->getOfferUnitPrice());
            result += DIVIDER;
            result += to_string((temp->getOffers())[i]->getOfferAmount());
            result += NEXT_LINE;
        }
        if (j != indicator)
            result += ORDER_DIVIDER;
    }
    return result;
}
string Manager::processLogout()
{
    if (onlineUser == NULL)
        throw permissionDenied();
    int offerId;
    if (pendingCart != NULL)
    {
        vector<Offer*> wantedOffers = pendingCart->getOffers();
        for (int i = 0; i < pendingCart->getCartSize(); i++)
        {
            offerId = wantedOffers[i]->getOfferId();
            offers[offerId - 1]->editOffer(offers[offerId - 1]->getOfferUnitPrice(), offers[offerId - 1]->getOfferAmount() + (pendingCart->getQuantities())[i]);
            ((Seller*)onlineUser)->changeOffer(offerId, offers[offerId - 1]->getOfferUnitPrice(), offers[offerId - 1]->getOfferAmount() + (pendingCart->getQuantities())[i]);
        }
    }
    pendingCart = NULL;
    onlineUser = NULL;
    return OK;
}
string Manager::processSubmitCart(vector<string> commanParts)
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)   
        throw permissionDenied();
    if (pendingCart == NULL)
        throw badRequest();
    float totalPrice = 0;
    float sellerProfit;
    vector<Offer*> wantedOffers = pendingCart->getOffers();
    for (int i = 0; i < pendingCart->getCartSize(); i++)
        totalPrice += wantedOffers[i]->getOfferUnitPrice() * (pendingCart->getPercentage())[i] * (pendingCart->getQuantities())[i];
    if (totalPrice > ((Buyer*)onlineUser)->calculateWallet())
        throw badRequest();
    else
        ((Buyer*)onlineUser)->addTransaction(-1 * totalPrice);
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->getUserType() == SELLER_TYPE)
        {
            sellerProfit = 0;
            for (int j = 0; j < pendingCart->getCartSize(); j++)
                if (users[i]->getUserId() == getSellerOfOffer((pendingCart->getOffers())[j]->getOfferId()))
                    sellerProfit += wantedOffers[j]->getOfferUnitPrice() * (pendingCart->getPercentage())[j]
                    * (pendingCart->getQuantities())[j];
            if (sellerProfit != 0)
                ((Seller*)users[i])->addTransaction(sellerProfit);
        }            
    }
    ((Buyer*)onlineUser)->addOrder(pendingCart);
    pendingCart = NULL;
    return OK;
}
string Manager::processGetProducts()
{
    string result = GET_PRODUCTS_TITLE;
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() == ADMIN_TYPE)
        throw permissionDenied();
    for (int i = 0; i < products.size(); i++)
    {
        result += to_string(products[i]->getProductId());
        result += DIVIDER;
        result += products[i]->getProductName();
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processLogin(vector<string> commandParts)
{
    int userIndex = searchEmail(commandParts[4]);
    if (onlineUser != NULL)
        throw badRequest();
    if (userIndex == NOT_FOUND)
        throw badRequest();
    if (users[userIndex]->getPassword() != wordHasher(commandParts[6]))
        throw badRequest();
    onlineUser = users[userIndex];
    return OK;
}
string Manager::processChargeWallet(vector<string> commandParts)
{
    float value = stof(commandParts[4]);
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    if (value <= 0)
        throw badRequest();
    ((Buyer *)onlineUser)->addTransaction(value);
    return OK;
}
string Manager::processOffers()
{
    if (onlineUser == NULL)
        throw permissionDenied();
    if (onlineUser->getUserType() != BUYER_TYPE)
        throw permissionDenied();
    string result = OFFERS_TITLE;
    for (int i = 0; i < offers.size(); i++)
    {
        result += to_string(offers[i]->getProductId());
        result += DIVIDER;
        result += products[offers[i]->getProductId()]->getProductName();
        result += DIVIDER;
        result += to_string(offers[i]->getOfferId());
        result += DIVIDER;
        result += to_string(offers[i]->getOfferUnitPrice());
        result += DIVIDER;
        result += to_string(offers[i]->getOfferAmount());
        result += NEXT_LINE;
    }
    return result;
}
string Manager::processGETCommand(vector<string> commandParts)
{
    int commandSize = commandParts.size();
    if (!isValidCommand(commandParts[1]))
        throw notFound();
    if ((commandSize == 2) && (commandParts[1] == "getProducts"))
        return processGetProducts();
    else if ((commandSize == 5) && (commandParts[1] == "wallet")  && (commandParts[2] == QUESTION_MARK) && (commandParts[3] == "count"))
        return processWallet(commandParts);
    else if ((commandSize == 2) && (commandParts[1] == "myOffers"))
        return processMyOffers();
    else if ((commandSize == 5) && (commandParts[1] == "productDetail") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "productId"))
        return processProductDetail(stoi(commandParts[4]) - 1);
    else if ((commandSize == 7) && (commandParts[1] == "compare")
    && (commandParts[2] == QUESTION_MARK) && (commandParts[3] == "productId1") && (commandParts[5] == "productId2"))
        return processCompare(stoi(commandParts[4]) - 1, stoi(commandParts[6]) - 1);
    else if ((commandSize == 5) && (commandParts[1] == "comments") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "productId"))
        return processComments(commandParts);
    else if ((commandSize == 7) && (commandParts[1] == "offers") && (commandParts[2] == QUESTION_MARK) && (commandParts[3] == "order")
    && ((commandParts[4] == "ASCEND") || (commandParts[4] == "DESCEND")) && (commandParts[5] == "field")
    && ((commandParts[6] == "offerId") || (commandParts[6] == "offerPrice")))
        return processOffers(commandParts);
    else if ((commandSize == 5) && (commandParts[1] == "offersOnProduct") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "productId"))
        return processOffersOnProduct(commandParts);
    else if ((commandSize == 2) && (commandParts[1] == "cart"))
        return processCart(commandParts);
    else if ((commandSize == 5) && (commandParts[1] == "orders") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "count"))
        return processOrders(commandParts);
    else if ((commandSize == 2) && (commandParts[1] == "offers"))
        return processOffers(commandParts);
    else
        throw badRequest();
}
string Manager::processPOSTCommand(vector<string> commandParts)
{
    int commandSize = commandParts.size();
    if (!isValidCommand(commandParts[1]))
        throw notFound();
    if ((commandSize == 7) && (commandParts[1] == "importProducts") && (commandParts[2] == QUESTION_MARK) && (commandParts[3] == "type")
    && (commandParts[5] == "filePath"))
        return processImportProduct(commandParts[6], commandParts[4]);
    else if (((commandSize == 9)||(commandSize == 11)) && (commandParts[1] == "signup") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "email") && (commandParts[5] == "username") && (commandParts[7] == "password"))
        return processSignup(commandParts);
    else if ((commandSize == 7) && (commandParts[1] == "login") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "email") && (commandParts[5] == "password"))
        return processLogin(commandParts);
    else if ((commandSize >= 7) && (commandParts[1] == "comment") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "productId") && (commandParts[5] == "comment"))
        return processComment(commandParts);
    else if ((commandSize == 5) && (commandParts[1] == "chargeWallet") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "amount"))
        return processChargeWallet(commandParts);
    else if ((commandSize == 2) && (commandParts[1] == "logout"))
        return processLogout();
    else if ((commandSize == 9) && (commandParts[1] == "offer") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "productId") && (commandParts[5] == "offerUnitPrice") && (commandParts[7] == "offerAmount"))
        return processOffer(commandParts);
    else if ((commandSize == 9) && (commandParts[1] == "changeOffer") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "offerId") && (commandParts[5] == "offerUnitPrice") && (commandParts[7] == "offerAmount"))
        return processChangeOffer(commandParts);
    else if ((commandSize == 9) && (commandParts[1] == "generateDiscountCode") && (commandParts[2] == QUESTION_MARK)
    && (commandParts[3] == "offerId") && (commandParts[5] == "discountPercent") && (commandParts[7] == "discountNumber"))
        return processGenerateDiscountCode(commandParts);
    else if (((commandSize == 7) || (commandSize == 9)) && (commandParts[1] == "addToCart") && (commandParts[2] == QUESTION_MARK) 
    && (commandParts[3] == "offerId") && (commandParts[5] == "amount"))
        return processAddToCart(commandParts);
    else if ((commandSize == 2) && (commandParts[1] == "submitCart"))
        return processSubmitCart(commandParts);
    else
        throw badRequest();
}
string Manager::processCommand(string command)
{
    vector<string> commandParts = parseInput(command, SPACE);
    int commandSize = commandParts.size();
    if (commandSize < 2)
        throw badRequest();
    if (commandParts[0] == "POST")
        return processPOSTCommand(commandParts);
    else if (commandParts[0] == "GET")
        return processGETCommand(commandParts);
    else 
        throw badRequest();
}