#ifndef __FUNC_H__
#define __FUNC_H__

#include <string>
#include <map>
#include <vector>
#include <apis.h>

class Parser {
private: 
    int argc;
    char** argv;
    std::vector<std::string> args;
public                       : 
Parser(int argc, char** argv): argc(argc), argv(argv) {}
    void getArgs();
    void parse();
    void printUsageAndExit();
};

class Blob {
    std::string content;
    std::string sha1;
public                          : 
Blob(const std::string& content): content(content) {
    sha1 = UsefulApi::hash(content);
}
    std::string getContent() const;
    std::string getSha1() const;
};

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
~Commit() {
    // TODO
}    

    void writeTreeCommitLogHEAD();
};


class Commands {
    std:: string currentPath;
    bool checkNitDirectory();
    void checkInit();

    Commit * commitCommand;
public: 
    /** 
    * @param path: current working directory
    */
    Commands(std::string path): currentPath(path), commitCommand(nullptr) {}

    ~Commands() {
        delete commitCommand;
    }
    void init();
    void status();
    void add();
    void checkout();
    void log();
    void commit(const std::string& msg);
};

#endif 