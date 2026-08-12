#ifndef MY_REFLECTOR_
#define MY_REFLECTOR_

#include <vector>

class Reflector
{
public:
    explicit Reflector(const std::vector<char> &s) : symbols(s) {};

    char reflect(const char symbol) const {
        return symbols[symbol];
    };
private:
    std::vector<char> symbols;
};

#endif // MY_REFLECTOR_