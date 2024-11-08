#include "io.h"

#include <iostream>

std::vector<uint8_t> readFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    std::vector<uint8_t> buffer;

    if (in.is_open()) {
        char tmp;
        in.seekg(0, std::ios::beg);
        while (in.get(tmp))
            buffer.push_back(static_cast<uint8_t>(tmp));
    }

    in.close();

    return buffer;
}

void writeToFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream out;
    out.open(filename, std::ios_base::binary);

    if (out.is_open())
        for (auto sym : data)
            out.put(static_cast<char>(sym));

    out.close();
}

void print(const std::vector<uint8_t>& data) {
    for (const auto& c: data)
        std::cout << c << " ";
    std::cout << std::endl;
}