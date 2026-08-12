#pragma once

#include <fstream>
#include <vector>

std::vector<uint8_t> readFromFile(const std::string &filename);
void writeToFile(const std::string& filename, const std::vector<uint8_t>& data);
void print(const std::vector<uint8_t>& data);