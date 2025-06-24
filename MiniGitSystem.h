#pragma once

#include "StagingArea.h"
#include <string>

class MiniGitSystem {
private:
    std::string currentBranch;
    StagingArea stagingArea;

    void loadHead();
    void updateHead(const std::string& commitHash);
    void restoreFiles(const std::string& commitHash);

public:
    MiniGitSystem();
    void init();
    void addFile(const std::string& filename);
    void commit(const std::string& message);
    void log();
    void branch(const std::string& branchName);
    void checkout(const std::string& name);
    void merge(const std::string& branchName);
    void diff(const std::string& commitHash1, const std::string& commitHash2);
};
