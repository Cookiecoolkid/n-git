#ifndef __COMMIT_H__
#define __COMMIT_H__

#include <string>
#include <map>
#include <vector>
#include <apis.h>
#include <iostream>
#include "func.h"
#include "tree.h"

class Commit {
    const std::string msg           = "";
    const std::string commitContent = "";
    const std::string commitSha1    = "";
    const std::string commitPath    = "";
    const std::string parentSha1    = "";
    Tree  * tree                    = nullptr;
    
public                                                                                                                                      : 
Commit(const std::string& msg, const std :: string& commitContent, const std::string& commitSha1, 
    const std::string& commitPath, const std::string& parentSha1, Tree* tree):
    msg(msg), commitContent(commitContent), commitSha1(commitSha1), commitPath(commitPath),
    parentSha1(parentSha1), tree(tree) {}
       
    void writeTreeCommitLogHEAD();
};

#endif