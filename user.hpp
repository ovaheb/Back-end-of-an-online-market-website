#ifndef __USER_H__
#define __USER_H__

#include "libraries.hpp"

size_t wordHasher(string word);

class User
{
public:
    User(int id, string name, string pass, string mail, int type)
        : userId(id), username(name), password(wordHasher(pass)), email(mail), userType(type) {}
    int getUserId() { return userId; }
    string getUsername() { return username; }
    size_t getPassword() { return password; }
    string getEmail() { return email; }
    int getUserType() { return userType; }
protected:
    int userId;
    string username;
    size_t password;
    string email;
    int userType;
};

#endif