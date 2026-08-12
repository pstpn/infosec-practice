#include "aes.hpp"

block AES::EncryptBlock(block& block128, const key& key128) {
    const auto keys = GetKeys128(key128);

    auto state = ToMatrix(block128);

    AddRoundKey(state, keys[0]);

    for (int i = 1; i < 10; ++i) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, keys[i]);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, keys[10]);

    return ToArray(state);
}

block AES::DecryptBlock(block& block128, const key& key128) {
    const auto keys = GetKeys128(key128);

    auto state = ToMatrix(block128);

    AddRoundKey(state, keys[10]);

    for (int i = 9; i > 0; --i) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, keys[i]);
        InvMixColumns(state);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, keys[0]);

    return ToArray(state);
}

key AES::sub_word(const key &word) {
    vector<uint8_t> result(word.size());

    for (size_t i = 0; i < word.size(); ++i)
        result[i] = S_BOX[word[i]];

    return result;
}

key AES::rot_word(const key &word) {
    vector<uint8_t> result = word;
    const uint8_t temp = result[0];

    for (size_t i = 0; i < word.size() - 1; ++i)
        result[i] = result[i + 1];

    result[word.size() - 1] = temp;

    return result;
}

vector<key> AES::GetKeys128(key key128) {
    vector roundKeys(11, key(16));

    roundKeys[0] = std::move(key128);

    for (int i = 1; i <= 10; ++i) {
        key tempWord = roundKeys[i - 1];

        tempWord = rot_word(tempWord);
        tempWord = sub_word(tempWord);
        tempWord[0] ^= R_CON[i - 1];

        for (int j = 0; j < 16; ++j)
            roundKeys[i][j] = roundKeys[i - 1][j] ^ tempWord[j];
    }

    return roundKeys;
}

void AES::AddRoundKey(matrix& block, const key& roundKey) {
    for (int i = 0; i < block.size(); ++i)
        for (int j = 0; j < block[0].size(); ++j)
            block[i][j] ^= roundKey[i + j];
}

void AES::InvSubBytes(matrix &block) {
    for (size_t i = 0; i < block.size(); ++i)
        for (int j = 0; j < block[0].size(); ++j)
            block[i][j] = S_BOX_INV[block[i][j]];
}

void AES::SubBytes(matrix &block) {
    for (size_t i = 0; i < block.size(); ++i)
        for (int j = 0; j < block[0].size(); ++j)
            block[i][j] = S_BOX[block[i][j]];
}

matrix AES::ToMatrix(const block &block) {
    matrix mtx(4, vector<uint8_t>(4));

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            mtx[j][i] = block[i * 4 + j];

    return mtx;
}

block AES::ToArray(const matrix &mtx) {
    block block128(16);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            block128[(i * 4) + j] = mtx[j][i];

    return block128;
}

void AES::ShiftRows(matrix &mtx) {
    for (int i = 1; i < mtx.size(); ++i)
        std::rotate(mtx[i].begin(), mtx[i].begin() + i, mtx[i].end());
}

void AES::InvShiftRows(matrix &mtx) {
    for (int i = 1; i < mtx.size(); ++i)
        std::rotate(mtx[i].rbegin(), mtx[i].rbegin() + i, mtx[i].rend());
}

void AES::MixColumns(matrix& mtx) {
    for (size_t i = 0; i < 4; ++i) {
        uint8_t s0 = mtx[0][i];
        uint8_t s1 = mtx[1][i];
        uint8_t s2 = mtx[2][i];
        uint8_t s3 = mtx[3][i];

        mtx[0][i] = GFMultiply(s0, 0x02) ^ GFMultiply(s1, 0x03) ^ s2 ^ s3;
        mtx[1][i] = s0 ^ GFMultiply(s1, 0x02) ^ GFMultiply(s2, 0x03) ^ s3;
        mtx[2][i] = s0 ^ s1 ^ GFMultiply(s2, 0x02) ^ GFMultiply(s3, 0x03);
        mtx[3][i] = GFMultiply(s0, 0x03) ^ s1 ^ s2 ^ GFMultiply(s3, 0x02);
    }
}

void AES::InvMixColumns(matrix &mtx) {
    for (size_t i = 0; i < 4; ++i) {
        uint8_t s0 = mtx[0][i];
        uint8_t s1 = mtx[1][i];
        uint8_t s2 = mtx[2][i];
        uint8_t s3 = mtx[3][i];

        mtx[0][i] = GFMultiply(s0, 0x0E) ^ GFMultiply(s1, 0x0B) ^ GFMultiply(s2, 0x0D) ^ GFMultiply(s3, 0x09);
        mtx[1][i] = GFMultiply(s0, 0x09) ^ GFMultiply(s1, 0x0E) ^ GFMultiply(s2, 0x0B) ^ GFMultiply(s3, 0x0D);
        mtx[2][i] = GFMultiply(s0, 0x0D) ^ GFMultiply(s1, 0x09) ^ GFMultiply(s2, 0x0E) ^ GFMultiply(s3, 0x0B);
        mtx[3][i] = GFMultiply(s0, 0x0B) ^ GFMultiply(s1, 0x0D) ^ GFMultiply(s2, 0x09) ^ GFMultiply(s3, 0x0E);
    }
}

uint8_t AES::GFMultiply(uint8_t x, uint8_t y) {
    uint8_t result = 0;
    uint8_t hbit = 0;

    for (int i = 0; i < 8; ++i) {
        if (y & 1)
            result ^= x;

        hbit = x & 0x80;
        x <<= 1;
        if (hbit)
            x ^= 0x1B;

        y >>= 1;
    }

    return result;
}
