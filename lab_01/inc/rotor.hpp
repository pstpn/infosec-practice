#ifndef MY_ROTOR_
#define MY_ROTOR_

#include <vector>

class Rotor
{
public:
    explicit Rotor(const std::vector<char> &symbols, const char start = 0)
        : start(start), shift(start), symbols(symbols) {};

    char get_symbol(const char index) const {
        if (index >= 0 && index < symbols.size())
            return symbols[(index + shift) % symbols.size()];
        return -1;
    }

    char get_index(const char symbol) const {
        const auto it = std::find(symbols.begin(), symbols.end(), symbol);
        if (it != symbols.end()) {
            const char i = std::distance(symbols.begin(), it);
            return (i - shift + symbols.size()) % symbols.size();
        }
        return -1;
    }

    char get_start() const {
        return start;
    }

    char rotate()& {
        shift = (shift + 1) % symbols.size();
        return shift;
    }

    void reset() {
        shift = start;
    }
private:
    char start;
    char shift;
    std::vector<char> symbols;
};

#endif // MY_ROTOR_
