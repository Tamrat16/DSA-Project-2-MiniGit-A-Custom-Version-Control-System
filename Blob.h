#pragma once

#include <string>

class Blob {
public:
    static std::string computeHash(const std::string& content);
    static void storeBlob(const std::string& content, const std::string& hash);
    static std::string readBlob(const std::string& hash);
};
