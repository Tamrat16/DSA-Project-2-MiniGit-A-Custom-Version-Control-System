#pragma once

#include <string>
#include <set>

class StagingArea {
private:
    std::set<std::string> files;

public:
    void add(const std::string& filename);
    void clear();
    const std::set<std::string>& getFiles() const;
};
