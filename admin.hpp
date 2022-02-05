#ifndef __ADMIN_H__
#define __ADMIN_H__

#include "libraries.hpp"
#include "user.hpp"

class Admin : public User
{
public:
    Admin()
        : User(0, "admin", "admin", "admin@gmail.com", ADMIN_TYPE) {}
private:

};

#endif