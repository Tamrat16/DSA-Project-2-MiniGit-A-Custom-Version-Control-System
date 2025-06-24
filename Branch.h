#pragma once

#include <string>

class Branch {
public:
    static void createBranch(const std::string& name, const std::string& commitHash);
    static std::string getBranchCommit(const std::string& name);
};
