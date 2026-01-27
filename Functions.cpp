#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
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

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // Ensure the vector has enough lines (pad with empty strings if necessary)
    while (lines.size() < lineToChange) {
        lines.push_back("");
    }

    // Replace the specified line (1-based index)
    lines[lineToChange - 1] = newData;

    // Write all lines to the temp file
    for (const auto& l : lines) {
        outFile << l << '\n';
    }
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

void makeNewFile(std::string fileLocation) {
    namespace fs = std::filesystem;
    fs::path outputPath(fileLocation);
    
    if (fs::exists(outputPath)) {
        std::cerr << "File already exists: " << fileLocation << std::endl;
        return;
    }

    std::ofstream outFile(fileLocation, std::ios::trunc);
    if (!outFile) {
        std::cerr << "Error creating file: " << fileLocation << std::endl;
        return;
    }
    outFile.close();
}

void makeNewFileInDirectory(std::string fileLocation) { //this version makes the directories if they don't exist. keep in mind that this may require elevated permissions depending on the path
    namespace fs = std::filesystem;
    fs::path outputPath(fileLocation);
    
    if (fs::exists(outputPath)) {
        std::cerr << "File already exists: " << fileLocation << std::endl;
        return;
    }

    fs::create_directories(outputPath.parent_path());
    std::ofstream outFile(fileLocation, std::ios::trunc);

    if (!outFile) {
        std::cerr << "Error creating file: " << fileLocation << std::endl;
        return;
    }

    outFile.close();
}