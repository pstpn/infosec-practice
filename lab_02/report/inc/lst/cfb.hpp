#pragma once

#include <iostream>

#include "des.hpp"

using namespace std;

typedef bitset<64> bitset64;
typedef bitset<56> bitset56;
typedef bitset<48> bitset48;
typedef bitset<32> bitset32;
typedef bitset<28> bitset28;

class CBF {
public:
    template<size_t BlockSize>
    vector<bitset<BlockSize>> encrypt(const vector<bitset<BlockSize>>& plaintext_blocks, const bitset64 key, const bitset64 IV) {
        vector<bitset<BlockSize>> ciphertext_blocks;
        bitset64 feedback = IV;

        for (auto &block : plaintext_blocks) {
            bitset64 encrypted_feedback = DES().encrypt(feedback, key);
            bitset<BlockSize> extended_encrypted_feedback = bitset<BlockSize>(encrypted_feedback.to_ullong());

            bitset<BlockSize> ciphertext_block = XOR(block, extended_encrypted_feedback);
            ciphertext_blocks.push_back(ciphertext_block);

            feedback = (feedback << BlockSize) | bitset64(ciphertext_block.to_ullong());
        }

        return ciphertext_blocks;
    }

    template<size_t BlockSize>
    vector<bitset<BlockSize>> decrypt(const vector<bitset<BlockSize>>& ciphertext_blocks, const bitset64 key, const bitset64 IV) {
        vector<bitset<BlockSize>> decrypted_blocks;
        bitset64 feedback = IV;

        for (auto &block : ciphertext_blocks) {
            bitset64 encrypted_feedback = DES().encrypt(feedback, key);
            bitset<BlockSize> extended_encrypted_feedback = bitset<BlockSize>(encrypted_feedback.to_ullong());

            bitset<BlockSize> plaintext_block = XOR(block, extended_encrypted_feedback);
            decrypted_blocks.push_back(plaintext_block);

            feedback = (feedback << BlockSize) | bitset64(block.to_ullong());
        }

        return decrypted_blocks;
    }

    template<size_t BlockSize>
    void encryptFile(const std::string& inputFile, const std::string& outputFile, bitset64 IV, bitset64 key, char t) {
        ifstream in;
        ofstream out(outputFile);;

        if (t == 't')
            in = ifstream(inputFile);
        else
            in = ifstream(inputFile, ios::binary);

        if (!in.is_open() || !out.is_open()) {
            throw runtime_error("Не удалось открыть файл.");
        }

        vector<bitset<BlockSize>> blocks;
        long blockSizeBytes = BlockSize / 8;
        vector<char> buffer(blockSizeBytes);

        while (in.read(buffer.data(), blockSizeBytes))
            blocks.push_back(charArrayToBitset<BlockSize>(buffer, blockSizeBytes));

        if (in.gcount() > 0) {
            memset(buffer.data() + in.gcount(), 0, blockSizeBytes - in.gcount());
            blocks.push_back(charArrayToBitset<BlockSize>(buffer, blockSizeBytes));
        }

        auto encrypted_blocks = encrypt(blocks, key, IV);

        for (const auto& block : encrypted_blocks)
            if (t == 't')
                out << bitsetToASCII(block);
            else
                out.write(bitsetToBytes(block).data(), bitsetToBytes(block).size());

        in.close();
        out.close();
    }

    template<size_t BlockSize>
    void decryptFile(const std::string& inputFile, const std::string& outputFile,  bitset64 IV, bitset64 key, char t) {
        ifstream in;
        ofstream out(outputFile);;

        if (t == 't')
            in = ifstream(inputFile);
        else
            in = ifstream(inputFile, ios::binary);

        if (!in.is_open() || !out.is_open()) {
            throw runtime_error("Не удалось открыть файл.");
        }

        vector<bitset<BlockSize>> blocks;
        long blockSizeBytes = BlockSize / 8;
        vector<char> buffer(blockSizeBytes);

        while (in.read(buffer.data(), blockSizeBytes))
            blocks.push_back(charArrayToBitset<BlockSize>(buffer, blockSizeBytes));

        auto decrypted_blocks = decrypt(blocks, key, IV);

        for (const auto& block : decrypted_blocks)
            if (t == 't')
                out << bitsetToASCII(block);
            else
                out.write(bitsetToBytes(block).data(), bitsetToBytes(block).size());

        in.close();
        out.close();
    }
private:
    template<size_t BlockSize>
    static bitset<BlockSize> XOR(bitset<BlockSize> a, bitset<BlockSize> b) {
        return a ^ b;
    }

    template<size_t BlockSize>
    bitset<BlockSize> charArrayToBitset(vector<char> array, const size_t size) {
        uint64_t value = 0;

        for (size_t i = 0; i < size; ++i)
            value = (value << 8) | static_cast<unsigned char>(array[i]);

        return bitset<BlockSize>(value);
    }

    template<size_t N>
    std::string bitsetToASCII(const std::bitset<N>& b) {
        std::string asciiString;

        for (int i = N - 1; i >= 0; i -= 8) {
            char c = 0;

            for (int j = 0; j < 8; ++j)
                c = (c << 1) | b[i - j];

            asciiString += c;
        }

        return asciiString;
    }

    template<size_t N>
    std::vector<char> bitsetToBytes(const std::bitset<N>& b) {
        std::vector<char> bytes(N / 8);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < 8; ++j)
                bytes[i] = (bytes[i] << 1) | b[N - 1 - i * 8 - j];

        return bytes;
    }
};
