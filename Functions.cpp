#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Functions.h"

#define successMessage //comment this out to stop the success message at the end of the fuctions

void changeInFile(std::string fileLocation, int lineToChange, std::string newData) {
    namespace fs = std::filesystem;
    fs::path inputPath(fileLocation);

    if (!fs::exists(inputPath)) {
        std::cerr << "Input file does not exist: " << fileLocation << std::endl;
        return;
    }

    // Create temp file in same directory as original to avoid permission issues on root
    fs::path tempPath = inputPath;
    tempPath += ".tmp";

    std::ifstream inFile(fileLocation);
    std::ofstream outFile(tempPath, std::ios::trunc);

    if (!inFile) {
        std::cerr << "Error opening input file: " << fileLocation << std::endl;
        return;
    }
    if (!outFile) {
        std::cerr << "Error creating temporary file: " << tempPath.string() << std::endl;
        return;
    }

    std::string line;
    int currentLine = 1;

    // Read every line from the original file and write to temp
    while (std::getline(inFile, line)) {
        if (currentLine == lineToChange) {
            outFile << newData << '\n';
        }
        else {
            outFile << line << '\n';
        }
        currentLine++;
    }

    inFile.close();
    outFile.close();

    std::error_code ec;
    // Remove original file
    if (!fs::remove(inputPath, ec)) {
        std::cerr << "Error deleting original file: " << ec.message() << std::endl;
        // try to remove temp to avoid littering
        fs::remove(tempPath, ec);
        return;
    }

    // Rename temp to original
    fs::rename(tempPath, inputPath, ec);
    if (ec) {
        std::cerr << "Error renaming temporary file: " << ec.message() << std::endl;
        return;
    }

#ifdef successMessage
    std::cout << "File updated successfully!\n";
#endif
}

std::string readLineFromFile(std::string fileLocation, int lineToRead) {
    std::ifstream inFile(fileLocation);
    if (!inFile) {
        std::cerr << "Error opening file: " << fileLocation << std::endl;
        return "";
    }
    std::string line;
    int currentLine = 1;
    while (std::getline(inFile, line)) {
        if (currentLine == lineToRead) {
            inFile.close();
            return line;
        }
        currentLine++;
    }
    inFile.close();
    std::cerr << "Line " << lineToRead << " does not exist in file: " << fileLocation << std::endl;
    return "";
}
