#ifndef __TREE_H__
#define __TREE_H__

#include <string>
#include <map>
#include <vector>
#include <apis.h>
#include "func.h"
#include <iostream>

class Tree {
    // TODO subTrees for sub directories
    // std::vector<Tree*> subTrees;
    // std::vector<std::pair<std::string, std::string>> files;
    std::string treeSha1;
    std::string treeContent;
    std::string treePath;

    // TODO
    std::map<std::string, std::string> files;  // filename -> fileSha1

public:
    Tree(const std::string& treeSha1, const std::string& treeContent, const std::string& treePath)
        : treeSha1(treeSha1), treeContent(treeContent), treePath(treePath) {}

    void addFile(const std::string& fileName, const std::string& sha1);

    std::string getTreeSha1() const;
    std::string getTreeContent() const;
    std::string getTreePath() const;
    const std::map<std::string, std::string>& getFiles() const;
};



#endif