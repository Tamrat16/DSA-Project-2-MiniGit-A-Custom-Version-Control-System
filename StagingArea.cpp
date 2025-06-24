#include "StagingArea.h"

void StagingArea::add(const std::string& filename) {
    files.insert(filename);
}

void StagingArea::clear() {
    files.clear();
}

const std::set<std::string>& StagingArea::getFiles() const {
    return files;
}
