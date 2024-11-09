#pragma once

#include <vector>

#include "aes.hpp"

class PCBC {
public:
    std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& plaintext, const block& iv, const key& key128) {
        std::vector<uint8_t> ciphertext;
        ciphertext.reserve(plaintext.size());

        block prevCipherBlock = iv;
        AES aes;

        for (size_t i = 0; i < plaintext.size(); i += 16) {
            block b(16);
            const size_t blockSize = std::min<size_t>(16, plaintext.size() - i);
            std::copy_n(plaintext.begin() + i, blockSize, b.begin());

            for (size_t j = 0; j < 16; ++j)
                b[j] ^= prevCipherBlock[j];

            auto cipherBlock = aes.EncryptBlock(b, key128);
            ciphertext.insert(ciphertext.end(), cipherBlock.begin(), cipherBlock.end());

            for (size_t j = 0; j < 16; ++j)
                prevCipherBlock[j] = plaintext[i + j] ^ cipherBlock[j];
        }

        return ciphertext;
    }

    std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& ciphertext, const block& iv, const key& key128) {
        std::vector<uint8_t> plaintext;
        plaintext.reserve(ciphertext.size());

        block prevCipherBlock = iv;
        AES aes;

        for (size_t i = 0; i < ciphertext.size(); i += 16) {
            block cipherBlock(16);
            std::copy_n(ciphertext.begin() + i, 16, cipherBlock.begin());

            auto decryptedBlock = aes.DecryptBlock(cipherBlock, key128);

            for (size_t j = 0; j < 16; ++j)
                decryptedBlock[j] ^= prevCipherBlock[j];

            plaintext.insert(plaintext.end(), decryptedBlock.begin(), decryptedBlock.end());

            for (size_t j = 0; j < 16; ++j)
                prevCipherBlock[j] = decryptedBlock[j] ^ cipherBlock[j];
        }

        return plaintext;
    }
};
