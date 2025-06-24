#include "Blob.h"
#include <fstream>
#include <filesystem>
#include <functional>

std::string Blob::computeHash(const std::string& content) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(content));
}

void Blob::storeBlob(const std::string& content, const std::string& hash) {
    std::filesystem::create_directories(".minigit/objects");
    std::ofstream out(".minigit/objects/" + hash);
    out << content;
    out.close();
}

std::string Blob::readBlob(const std::string& hash) {
    std::ifstream in(".minigit/objects/" + hash);
    if (!in) return "";
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}
