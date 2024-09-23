#include "tree.h"

void Tree::addFile(const std::string& fileName, const std::string& sha1) {
    files[fileName] = sha1;
}

std::string Tree::getTreeSha1() const {
    return treeSha1;
}

std::string Tree::getTreeContent() const {
    return treeContent;
}

std::string Tree::getTreePath() const {
    return treePath;
}

const std::map<std::string, std::string>& Tree::getFiles() const {
    return files;
}
