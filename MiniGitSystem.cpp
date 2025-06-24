#define _CRT_SECURE_NO_WARNINGS

#include "MiniGitSystem.h"
#include "Blob.h"
#include "Commit.h"
#include "Branch.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <ctime>
#include <sstream>
#include <set>

MiniGitSystem::MiniGitSystem() {
    loadHead();
}

void MiniGitSystem::loadHead() {
    std::ifstream in(".minigit/HEAD");
    if (in) {
        getline(in, currentBranch);
    }
    else {
        currentBranch = "";
    }
}

void MiniGitSystem::updateHead(const std::string& commitHash) {
    if (currentBranch == "") {
        std::ofstream out(".minigit/HEAD");
        out << "master";
        currentBranch = "master";
    }
    Branch::createBranch(currentBranch, commitHash);
}

void MiniGitSystem::init() {
    if (std::filesystem::exists(".minigit")) {
        std::cout << "Repository already initialized." << std::endl;
        return;
    }
    std::filesystem::create_directories(".minigit/objects");
    std::filesystem::create_directories(".minigit/commits");
    std::filesystem::create_directories(".minigit/refs");
    std::ofstream headFile(".minigit/HEAD");
    headFile << "master";
    headFile.close();
    std::ofstream branchFile(".minigit/refs/master");
    branchFile << "";
    branchFile.close();
    std::cout << "Initialized empty MiniGit repository." << std::endl;
    loadHead();
}

void MiniGitSystem::addFile(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        std::cout << "File not found." << std::endl;
        return;
    }
    stagingArea.add(filename);
    std::cout << "File staged: " << filename << std::endl;
}

void MiniGitSystem::commit(const std::string& message) {
    std::map<std::string, std::string> filesMap;

    std::string parentCommit = Branch::getBranchCommit(currentBranch);
    if (!parentCommit.empty()) {
        std::ifstream in(".minigit/commits/" + parentCommit);
        std::string parent, msg, timestamp;
        getline(in, parent);
        getline(in, msg);
        getline(in, timestamp);
        std::string line;
        while (getline(in, line)) {
            size_t pos = line.find(" ");
            if (pos != std::string::npos) {
                std::string file = line.substr(0, pos);
                std::string hash = line.substr(pos + 1);
                filesMap[file] = hash;
            }
        }
        in.close();
    }

    for (const auto& file : stagingArea.getFiles()) {
        std::ifstream inFile(file);
        std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        std::string hash = Blob::computeHash(content);
        Blob::storeBlob(content, hash);
        filesMap[file] = hash;
    }

    std::string commitHash = Commit::createCommit(parentCommit, filesMap, message);
    updateHead(commitHash);
    stagingArea.clear();
    std::cout << "Commit created: " << commitHash << std::endl;
}

void MiniGitSystem::log() {
    std::string commitHash = Branch::getBranchCommit(currentBranch);
    while (!commitHash.empty()) {
        std::ifstream in(".minigit/commits/" + commitHash);
        if (!in) break;
        std::string parent, message, timestamp;
        getline(in, parent);
        getline(in, message);
        getline(in, timestamp);
        std::time_t ts = std::stoll(timestamp);
        char timeBuffer[26];
        ctime_s(timeBuffer, sizeof(timeBuffer), &ts);
        std::cout << "Commit: " << commitHash << std::endl;
        std::cout << "Date: " << timeBuffer;
        std::cout << "Message: " << message << std::endl;
        std::cout << "-------------------------" << std::endl;
        commitHash = parent;
    }
}

void MiniGitSystem::branch(const std::string& branchName) {
    std::string currentCommit = Branch::getBranchCommit(currentBranch);
    Branch::createBranch(branchName, currentCommit);
    std::cout << "Branch created: " << branchName << std::endl;
}

void MiniGitSystem::checkout(const std::string& name) {
    std::string targetCommit = Branch::getBranchCommit(name);
    if (targetCommit.empty() && std::filesystem::exists(".minigit/commits/" + name))
        targetCommit = name;

    if (targetCommit.empty()) {
        std::cout << "Branch or commit not found." << std::endl;
        return;
    }

    restoreFiles(targetCommit);
    if (std::filesystem::exists(".minigit/refs/" + name)) {
        currentBranch = name;
        std::ofstream out(".minigit/HEAD");
        out << name;
    }
    std::cout << "Checked out to: " << name << std::endl;
}

void MiniGitSystem::restoreFiles(const std::string& commitHash) {
    std::ifstream in(".minigit/commits/" + commitHash);
    std::string parent, message, timestamp;
    getline(in, parent);
    getline(in, message);
    getline(in, timestamp);
    std::string line;
    while (getline(in, line)) {
        size_t pos = line.find(" ");
        if (pos != std::string::npos) {
            std::string file = line.substr(0, pos);
            std::string hash = line.substr(pos + 1);
            std::ofstream out(file);
            out << Blob::readBlob(hash);
        }
    }
}

void MiniGitSystem::merge(const std::string& branchName) {
    std::cout << "Merge feature not fully implemented yet." << std::endl;
}

// NEW: Full diff function
void MiniGitSystem::diff(const std::string& commitHash1, const std::string& commitHash2) {
    std::ifstream in1(".minigit/commits/" + commitHash1);
    std::ifstream in2(".minigit/commits/" + commitHash2);

    if (!in1 || !in2) {
        std::cout << "One or both commits not found." << std::endl;
        return;
    }

    std::string dummy;
    for (int i = 0; i < 3; ++i) getline(in1, dummy);
    for (int i = 0; i < 3; ++i) getline(in2, dummy);

    std::map<std::string, std::string> files1, files2;

    std::string line;
    while (getline(in1, line)) {
        size_t pos = line.find(" ");
        files1[line.substr(0, pos)] = line.substr(pos + 1);
    }
    while (getline(in2, line)) {
        size_t pos = line.find(" ");
        files2[line.substr(0, pos)] = line.substr(pos + 1);
    }

    std::set<std::string> allFiles;
    for (auto& [f, h] : files1) allFiles.insert(f);
    for (auto& [f, h] : files2) allFiles.insert(f);

    for (const auto& file : allFiles) {
        std::string content1 = files1.count(file) ? Blob::readBlob(files1[file]) : "";
        std::string content2 = files2.count(file) ? Blob::readBlob(files2[file]) : "";

        if (content1 == content2) {
            std::cout << "File: " << file << " — No changes." << std::endl;
            continue;
        }

        std::cout << "File: " << file << " — Differences:" << std::endl;

        std::istringstream s1(content1);
        std::istringstream s2(content2);
        std::string line1, line2;
        int lineNum = 1;

        while (true) {
            bool has1 = (bool)std::getline(s1, line1);
            bool has2 = (bool)std::getline(s2, line2);

            if (!has1 && !has2) break;

            if (has1 && has2) {
                if (line1 != line2)
                    std::cout << "Line " << lineNum << ":\n  Commit1: " << line1 << "\n  Commit2: " << line2 << std::endl;
            }
            else if (has1) {
                std::cout << "Line " << lineNum << ":\n  Commit1: " << line1 << "\n  Commit2: [missing]" << std::endl;
            }
            else if (has2) {
                std::cout << "Line " << lineNum << ":\n  Commit1: [missing]\n  Commit2: " << line2 << std::endl;
            }
            lineNum++;
        }
        std::cout << "------------------------------" << std::endl;
    }
}
