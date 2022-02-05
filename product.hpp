#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "libraries.hpp"

class Product
{
public:
    Product(int id, string name, int type);
    int getProductId() { return productId; }
    int getProductType() { return productType; }
    string getProductName() { return productName; }
    vector<Comment> getComments() { return comments; }
    void addComment(Comment newComment);
    virtual vector<string> productDetail() = 0;
protected:
    int productId;
    int productType;
    string productName;
    vector<Comment> comments;
};


#endif