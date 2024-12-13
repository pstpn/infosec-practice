#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
#include <stdexcept>

class HuffmanNode {
public:
    unsigned char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(const unsigned char character, const int frequency)
        : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

class HuffmanCompression {
public:
    static void compress(const std::string& inputFile, const std::string& outputFile);
    static void decompress(const std::string& inputFile, const std::string& outputFile);

private:
    struct Compare {
        bool operator()(const HuffmanNode* l, const HuffmanNode* r) const {
            return l->freq > r->freq;
        }
    };

    static HuffmanNode* buildHuffmanTree(const std::map<unsigned char, int>& freqMap);
    static void buildCodes(const HuffmanNode* root, const std::string& str, std::map<unsigned char, std::string>& huffmanCode);
    static void writeCompressedFile(std::ofstream& outFile, const std::map<unsigned char, std::string>& huffmanCode, const std::string& inputFile);
    static void writeFrequencyTable(std::ofstream& outFile, const std::map<unsigned char, int>& freqMap);
    static std::map<unsigned char, int> readFrequencyTable(std::ifstream& inFile);
    static std::string readEncodedData(std::ifstream& inFile);
};

HuffmanNode* HuffmanCompression::buildHuffmanTree(const std::map<unsigned char, int>& freqMap) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (const auto&[fst, snd] : freqMap)
        pq.push(new HuffmanNode(fst, snd));

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        auto* node = new HuffmanNode(0, left->freq + right->freq);
        node->left = left;
        node->right = right;
        pq.push(node);
    }

    return pq.empty() ? nullptr : pq.top();
}

void HuffmanCompression::buildCodes(const HuffmanNode* root, const std::string& str, std::map<unsigned char, std::string>& huffmanCode) {
    if (!root)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str.empty() ? "0" : str;
        return;
    }

    buildCodes(root->left, str + "0", huffmanCode);
    buildCodes(root->right, str + "1", huffmanCode);
}


void HuffmanCompression::writeFrequencyTable(std::ofstream& outFile, const std::map<unsigned char, int>& freqMap) {
    uint32_t size = freqMap.size();
    outFile.write(reinterpret_cast<char*>(&size), sizeof(size));

    for (const auto&[fst, snd] : freqMap) {
        outFile.put(fst);
        outFile.write(reinterpret_cast<const char*>(&snd), sizeof(snd));
    }
}

std::map<unsigned char, int> HuffmanCompression::readFrequencyTable(std::ifstream& inFile) {
    std::map<unsigned char, int> freqMap;
    uint32_t size;
    int freq;

    if (!inFile.read(reinterpret_cast<char*>(&size), sizeof(size)))
        return {};

    for (uint32_t i = 0; i < size; ++i) {
        unsigned char ch = inFile.get();
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        freqMap[ch] = freq;
    }

    return freqMap;
}

void HuffmanCompression::writeCompressedFile(std::ofstream& outFile, const std::map<unsigned char, std::string>& huffmanCode, const std::string& inputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile)
        throw std::runtime_error("Cannot open input file for compression");

    std::string encodedData;
    char ch;
    while (inFile.get(ch))
        encodedData += huffmanCode.at(ch);
    inFile.close();

    const size_t originalBitLength = encodedData.size();
    while (encodedData.size() % 8)
        encodedData += '0';

    outFile.write(reinterpret_cast<const char*>(&originalBitLength), sizeof(originalBitLength));

    for (size_t i = 0; i < encodedData.size(); i += 8) {
        std::bitset<8> byte(encodedData.substr(i, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }
}

std::string HuffmanCompression::readEncodedData(std::ifstream& inFile) {
    size_t originalBitLength;
    inFile.read(reinterpret_cast<char*>(&originalBitLength), sizeof(originalBitLength));

    std::string encodedData;
    unsigned char byte;

    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        std::bitset<8> bits(byte);
        encodedData += bits.to_string();
    }

    encodedData = encodedData.substr(0, originalBitLength);

    return encodedData;
}

void HuffmanCompression::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile)
        throw std::runtime_error("Cannot open input file");

    std::map<unsigned char, int> freqMap;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1))
        freqMap[byte]++;
    inFile.close();

    std::ofstream outFile(outputFile, std::ios::binary);

    if (freqMap.empty()) {
        outFile.close();
        return;
    }

    HuffmanNode* root = buildHuffmanTree(freqMap);
    std::map<unsigned char, std::string> huffmanCode;
    buildCodes(root, "", huffmanCode);

    writeFrequencyTable(outFile, freqMap);
    writeCompressedFile(outFile, huffmanCode, inputFile);
    outFile.close();

    delete root;
}

void HuffmanCompression::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile)
        throw std::runtime_error("Cannot open input file");

    std::ofstream outFile(outputFile, std::ios::binary);

    std::map<unsigned char, int> freqMap = readFrequencyTable(inFile);
    if (freqMap.empty()) {
        inFile.close();
        outFile.close();
        return;
    }

    std::string encodedData = readEncodedData(inFile);
    inFile.close();

    HuffmanNode* root = buildHuffmanTree(freqMap);
    if (!root)
        throw std::runtime_error("Failed to rebuild Huffman tree");

    if (!root->left && !root->right)
        for (size_t i = 0; i < encodedData.size(); ++i)
            outFile.put(root->ch);
    else {
        HuffmanNode* current = root;

        for (char bit : encodedData) {
            current = (bit == '0') ? current->left : current->right;

            if (!current->left && !current->right) {
                outFile.put(current->ch);
                current = root;
            }
        }
    }

    outFile.close();
    delete root;
}

double calculateCompressionCoeff(const std::string& originalFile, const std::string& compressedFile) {
    std::ifstream original(originalFile, std::ios::binary | std::ios::ate);
    if (!original)
        throw std::runtime_error("Cannot open original file");
    std::streamsize originalSize = original.tellg();
    original.close();

    std::ifstream compressed(compressedFile, std::ios::binary | std::ios::ate);
    if (!compressed)
        throw std::runtime_error("Cannot open compressed file");
    std::streamsize compressedSize = compressed.tellg();
    compressed.close();

    if (originalSize == 0)
        return 0.0;

    return static_cast<double>(compressedSize) / static_cast<double>(originalSize);
}


int main() {
    try {
        /**
         * Text
        **/
        std::string inputFile = "/Users/stepa/Study/InfoSec/lab_05/data/file.txt";
        std::string compressedFile = "/Users/stepa/Study/InfoSec/lab_05/data/enc.txt";
        std::string decompressedFile = "/Users/stepa/Study/InfoSec/lab_05/data/dec.txt";

        HuffmanCompression::compress(inputFile, compressedFile);
        HuffmanCompression::decompress(compressedFile, decompressedFile);
        std::cout << "\n[TXT] Compress coeff: " << calculateCompressionCoeff(inputFile, compressedFile) << std::endl;

        /**
         * BMP
        **/
        inputFile = "/Users/stepa/Study/InfoSec/lab_05/data/file.bmp";
        compressedFile = "/Users/stepa/Study/InfoSec/lab_05/data/enc.bmp";
        decompressedFile = "/Users/stepa/Study/InfoSec/lab_05/data/dec.bmp";

        HuffmanCompression::compress(inputFile, compressedFile);
        HuffmanCompression::decompress(compressedFile, decompressedFile);
        std::cout << "[BMP] Compress coeff: " << calculateCompressionCoeff(inputFile, compressedFile) << std::endl;

        /**
         * ZIP
        **/
        inputFile = "/Users/stepa/Study/InfoSec/lab_05/data/file.zip";
        compressedFile = "/Users/stepa/Study/InfoSec/lab_05/data/enc.zip";
        decompressedFile = "/Users/stepa/Study/InfoSec/lab_05/data/dec.zip";

        HuffmanCompression::compress(inputFile, compressedFile);
        HuffmanCompression::decompress(compressedFile, decompressedFile);
        std::cout << "[ZIP] Compress coeff: " << calculateCompressionCoeff(inputFile, compressedFile) << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }

    return 0;
}