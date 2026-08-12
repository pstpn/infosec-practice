#include <iostream>
#include <fstream>

#include "enigma.hpp"
#include "rotor.hpp"

#define INPUT_TXT_FILE "/Users/stepa/Study/InfoSec/lab_01/data/file.txt"
#define OUTPUT_ENCRYPT_TXT_FILE "/Users/stepa/Study/InfoSec/lab_01/out/e_file.txt"
#define OUTPUT_DECRYPT_TXT_FILE "/Users/stepa/Study/InfoSec/lab_01/out/d_file.txt"

#define INPUT_ZIP_FILE "/Users/stepa/Study/InfoSec/lab_01/data/file.zip"
#define OUTPUT_ENCRYPT_ZIP_FILE "/Users/stepa/Study/InfoSec/lab_01/out/e_file.zip"
#define OUTPUT_DECRYPT_ZIP_FILE "/Users/stepa/Study/InfoSec/lab_01/out/d_file.zip"

#define INPUT_JPG_FILE "/Users/stepa/Study/InfoSec/lab_01/data/file.jpg"
#define OUTPUT_ENCRYPT_JPG_FILE "/Users/stepa/Study/InfoSec/lab_01/out/e_file.jpg"
#define OUTPUT_DECRYPT_JPG_FILE "/Users/stepa/Study/InfoSec/lab_01/out/d_file.jpg"

/**
 * CAPITAL ROFLS
 */
#define ZIP
#ifdef ZIP
#define INPUT_FILE INPUT_ZIP_FILE
#define OUTPUT_ENCRYPT_FILE OUTPUT_ENCRYPT_ZIP_FILE
#define OUTPUT_DECRYPT_FILE OUTPUT_DECRYPT_ZIP_FILE
#elifdef JPG
#define INPUT_FILE INPUT_JPG_FILE
#define OUTPUT_ENCRYPT_FILE OUTPUT_ENCRYPT_JPG_FILE
#define OUTPUT_DECRYPT_FILE OUTPUT_DECRYPT_JPG_FILE
#else
#define TXT
#define INPUT_FILE INPUT_TXT_FILE
#define OUTPUT_ENCRYPT_FILE OUTPUT_ENCRYPT_TXT_FILE
#define OUTPUT_DECRYPT_FILE OUTPUT_DECRYPT_TXT_FILE
#endif

int main() {
#ifdef TXT
    const std::vector<Rotor> rotors{
        Rotor{std::vector<char>{
            114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 72, 71, 70, 69, 68, 67, 66, 65, 102, 101, 100, 99,
            98, 97, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 96, 95, 94, 93, 92, 91, 90, 89, 88,
            87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73
        }, 5},
        Rotor{std::vector<char>{
            92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 114, 113, 112, 111, 110, 109,
            108, 107, 106, 105, 104, 103, 72, 71, 70, 69, 68, 67, 66, 65, 102, 101, 100, 99, 98, 97, 127, 126, 125, 124,
            123, 122, 121, 120, 119, 118, 117, 116, 115, 96, 95, 94, 93
        }, 30},
        Rotor{std::vector<char>{
            121, 127, 126, 125, 124, 123, 122, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106,
            105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80,
            79, 78, 77, 76, 75, 74, 73, 69, 68, 67, 66, 65, 72, 71, 70
        }, 10}
    };
    const Reflector reflector{std::vector<char>{
        127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105,
        104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78,
        77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65
    }};
    const Commutator commutator{std::vector<char>{
        127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105,
        104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78,
        77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65
    }};
    Enigma enigma{rotors, reflector, commutator, 'A'};

    /**
    Manual input
    **/
    std::string s;
    std::cout << "\nInput word: ";
    std::cin >> s;

    std::string encrypted_word = enigma.encrypt(s);
    std::cout << "\nEncrypted: " << encrypted_word << std::endl;
    std::cout << "Decrypted: " << enigma.decrypt(encrypted_word) << std::endl;

    std::ifstream infile(INPUT_FILE);
#else
    const std::vector<Rotor> rotors{
        Rotor{std::vector<char>{1, 0, 7, 5, 4, 3, 15, 14, 13, 10, 9, 8, 2, 6, 12, 11,}},
        Rotor{std::vector<char>{12, 11, 10, 15, 14, 13, 9, 8, 2, 1, 0, 7, 6, 5, 4, 3}},
        Rotor{std::vector<char>{5, 4, 3, 15, 14, 13, 12, 11, 10, 9, 8, 2, 1, 0, 7, 6}}
    };
    const Reflector reflector{std::vector<char>{15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};
    const Commutator commutator{std::vector<char>{15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};
    Enigma enigma{rotors, reflector, commutator, 0};
    std::ifstream infile(INPUT_FILE, std::ios::binary | std::ios::in);
#endif

    /**
    File input
    **/
    std::ofstream e_outfile(OUTPUT_ENCRYPT_FILE);
    std::ofstream d_outfile(OUTPUT_DECRYPT_FILE);
    std::string line;

    if (!infile.is_open()) {
        std::cerr << "Error: could not open input file: " << INPUT_FILE << std::endl;
        return -1;
    }
    if (!e_outfile.is_open()) {
        std::cerr << "Error: could not open output encrypt file: " << OUTPUT_ENCRYPT_FILE << std::endl;
        return -1;
    }
    if (!d_outfile.is_open()) {
        std::cerr << "Error: could not open output decrypt file: " << OUTPUT_DECRYPT_FILE << std::endl;
        return -1;
    }

#ifdef TXT
    while (std::getline(infile, line)) {
        enigma.reset();
        const std::string encrypted_line = enigma.encrypt(line);
        e_outfile << encrypted_line << std::endl;
        std::string decrypted_line = enigma.decrypt(encrypted_line);
        d_outfile << decrypted_line << std::endl;
    }
#else
    char c;
    while (infile.get(c)) {
        char e_res = 0, d_res = 0;

        char first_bits = (c & 0b11110000) >> 4; // Извлечение первых 4 бит (11110000)
        char second_bits = c & 0b00001111; // Извлечение вторых 4 бит (1111)

        enigma.reset();
        char e_four_bits = enigma.encrypt(std::string{first_bits})[0];
        char d_four_bits = enigma.decrypt(std::string{e_four_bits})[0];

        e_res |= e_four_bits << 4;
        d_res |= d_four_bits << 4;

        enigma.reset();
        e_four_bits = enigma.encrypt(std::string{second_bits})[0];
        d_four_bits = enigma.decrypt(std::string{e_four_bits})[0];

        e_res |= e_four_bits;
        d_res |= d_four_bits;

        e_outfile.write(&e_res, 1);
        d_outfile.write(&d_res, 1);
    }
#endif

    infile.close();
    e_outfile.close();
    d_outfile.close();

    return 0;
}