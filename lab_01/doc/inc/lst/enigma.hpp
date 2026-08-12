#ifndef MY_ENIGMA_
#define MY_ENIGMA_

#include <string>
#include <vector>

#include "rotor.hpp"
#include "commutator.hpp"
#include "reflector.hpp"

class Enigma {
public:
    explicit Enigma(std::vector<Rotor> rotors, Reflector reflector, Commutator commutator, const char &min_symbol)
        : rotors(std::move(rotors)), commutator(std::move(commutator)), reflector(std::move(reflector)), min_symbol(min_symbol) {}

    std::string encrypt(const std::string &word) {
        std::string output;

        for (const char symbol : word) {
            // Apply the commutator
            char current_symbol = commutator.commutate(symbol - min_symbol) - min_symbol;

            // Forward pass through the rotors
            for (Rotor &rotor : rotors) {
                current_symbol = rotor.get_index(current_symbol + min_symbol);
            }

            // Apply the reflector
            current_symbol = reflector.reflect(current_symbol);

            // Reverse pass through the rotors
            for (auto it = rotors.rbegin(); it != rotors.rend(); ++it) {
                current_symbol = it->get_symbol(current_symbol - min_symbol);
            }

            // Apply the commutator again
            current_symbol = commutator.commutate(current_symbol - min_symbol);

            // Convert back to the character range
            output.push_back(current_symbol);

            // Rotate the rotors
            rotate_rotors();
        }

        return output;
    }

    std::string decrypt(const std::string &word) {
        reset();
        return encrypt(word);
    }

    void reset() {
        for (Rotor &rotor : rotors)
            rotor.reset();
    }
private:
    std::vector<Rotor> rotors;
    Commutator commutator;
    Reflector reflector;

    char min_symbol;

    void rotate_rotors() {
        for (auto & rotor : rotors) {
            if (rotor.rotate() != rotor.get_start()) {
                break;
            }
        }
    }
};

#endif // MY_ENIGMA_
