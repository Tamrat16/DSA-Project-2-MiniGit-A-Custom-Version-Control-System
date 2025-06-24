#pragma once

#include <string>
#include <map>

class Commit {
public:
    static std::string createCommit(const std::string& parent, const std::map<std::string, std::string>& files, const std::string& message);
    static void readCommit(const std::string& hash);
};
