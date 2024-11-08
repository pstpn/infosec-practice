#include "aes.hpp"
#include "io.h"
#include "pcbc.hpp"

void blockCipherAES() {
    key key128 = { 65, 65, 65, 65, 65, 64, 67, 65, 65, 70, 65, 65, 65, 65, 65, 65 };
    std::vector<uint8_t> message = {
        'A', 'J', 'H', 'F', 'H', 'A', 'U', 'I', 'c', 'a', 'p', 'C',
        'U', 'I', 'A', 'H', 'C', 'P', 'I', 'H', 'S', 'I', 'U', 'C',
        'I', 'A', 0, 0, 0, 0, 0, 0
    };

    print(message);
    AES enc;
    block encr_block = enc.EncryptBlock(message, key128);
    print(encr_block);

    block decr_block = enc.DecryptBlock(encr_block, key128);
    print(decr_block);

    vector<uint8_t> message1 = vector(message.begin() + 16, message.end());
    block encr_block1 = enc.EncryptBlock(message1, key128);
    print(encr_block1);

    block decr_block1 = enc.DecryptBlock(encr_block1, key128);
    print(decr_block1);
}

int main() {
    // blockCipherAES();

    key key128 = { 100, 65, 65, 200, 65, 64,67, 0, 65, 70, 65, 65, 65, 65, 65, 65 };
    std::vector<uint8_t> message = {
        'A', 'J', 'H', 'F', 'H', 'A', 'U', 'I', 'c', 'a', 'p', 'C',
        'U', 'I', 'A', 'H', 'C', 'P', 'I', 'H', 'S', 'I', 'U', 'C',
        'I', 'A'
    };
    block iv = { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80 };

    PCBC pcbc;

    /**
     *
     * TEXT
     *
    **/
    vector<uint8_t> message1 = readFromFile("/Users/stepa/Study/InfoSec/lab_03/data/file.txt");
    vector<uint8_t> enc1 = pcbc.Encrypt(message1, iv, key128);
    writeToFile("/Users/stepa/Study/InfoSec/lab_03/data/enc.txt", enc1);
    message1 = readFromFile("/Users/stepa/Study/InfoSec/lab_03/data/enc.txt");
    writeToFile("/Users/stepa/Study/InfoSec/lab_03/data/dec.txt", pcbc.Decrypt(message1, iv, key128));

    /**
     *
     * BMP
     *
    **/
    vector<uint8_t> message2 = readFromFile("/Users/stepa/Study/InfoSec/lab_03/data/file.bmp");
    vector<uint8_t> enc2 = pcbc.Encrypt(message2, iv, key128);
    writeToFile("/Users/stepa/Study/InfoSec/lab_03/data/enc.bmp", enc2);
    message2 = readFromFile("/Users/stepa/Study/InfoSec/lab_03/data/enc.bmp");
    writeToFile("/Users/stepa/Study/InfoSec/lab_03/data/dec.bmp", pcbc.Decrypt(message2, iv, key128));

    /**
     *
     * ZIP
     *
    **/
    vector<uint8_t> message3 = readFromFile("/Users/stepa/Study/InfoSec/lab_03/data/file.zip");
    vector<uint8_t> enc3 = pcbc.Encrypt(message3, iv, key128);
    writeToFile("/Users/stepa/Study/InfoSec/lab_03/data/enc.zip", enc3);
    message3 = readFromFile("/Users/stepa/Study/InfoSec/lab_03/data/enc.zip");
    writeToFile("/Users/stepa/Study/InfoSec/lab_03/data/dec.zip", pcbc.Decrypt(message3, iv, key128));
}
