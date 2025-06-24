#include "Branch.h"
#include <fstream>
#include <filesystem>

void Branch::createBranch(const std::string& name, const std::string& commitHash) {
    std::filesystem::create_directories(".minigit/refs");
    std::ofstream out(".minigit/refs/" + name);
    out << commitHash;
    out.close();
}

std::string Branch::getBranchCommit(const std::string& name) {
    std::ifstream in(".minigit/refs/" + name);
    if (!in) return "";
    std::string commitHash;
    getline(in, commitHash);
    return commitHash;
}
