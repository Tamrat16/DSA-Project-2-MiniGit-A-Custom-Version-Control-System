#include "Commit.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>

std::string Commit::createCommit(const std::string& parent, const std::map<std::string, std::string>& files, const std::string& message) {
    std::string commitData = parent + "\n" + message + "\n" + std::to_string(std::time(0)) + "\n";
    for (const auto& file : files) {
        commitData += file.first + " " + file.second + "\n";
    }

    std::hash<std::string> hasher;
    std::string commitHash = std::to_string(hasher(commitData));

    std::filesystem::create_directories(".minigit/commits");
    std::ofstream out(".minigit/commits/" + commitHash);
    out << commitData;
    out.close();

    return commitHash;
}

void Commit::readCommit(const std::string& hash) {
    std::ifstream in(".minigit/commits/" + hash);
    if (!in) {
        std::cout << "Commit not found." << std::endl;
        return;
    }
    std::string line;
    while (getline(in, line)) {
        std::cout << line << std::endl;
    }
}
