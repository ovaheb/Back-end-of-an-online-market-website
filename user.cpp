#include "user.hpp"

size_t wordHasher(string word)
{
    hash<string> hasher;
    size_t result = hasher(word);
    return result;
}