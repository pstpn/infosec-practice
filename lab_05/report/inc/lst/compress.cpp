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