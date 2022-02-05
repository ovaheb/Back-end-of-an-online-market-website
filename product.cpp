#include "product.hpp"

Product::Product(int id, string name, int type)
{
    productId = id;
    productName = name;
    vector<Comment> temp;
    comments = temp;
    productType = type;
}
void Product::addComment(Comment newComment)
{
    comments.push_back(newComment);
}