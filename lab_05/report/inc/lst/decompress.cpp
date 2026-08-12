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