#ifndef MY_COMMUTATOR_
#define MY_COMMUTATOR_

#include <vector>

class Commutator
{
public:
    explicit Commutator(const std::vector<char>& symbols) : symbols(symbols) {};

    char commutate(const char &symbol) const {
        return symbols[symbol];
    };

private:
    std::vector<char> symbols;
};

#endif // MY_COMMUTATOR_