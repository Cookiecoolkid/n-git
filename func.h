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
    std::vector<std::pair<std::string, std::string>> files;

public: 
    void addFile(const std::string& fileName, std::string& sha1);
};

class Commit {
    const  std::string msg      = "";
    const  std::string commitId = "";
    Commit * parent             = nullptr;
    Tree   * tree               = nullptr;
    
public                                                                                 : 
Commit(const std::string& msg, const std::string& commitId, Commit* parent, Tree* tree): 
    msg(msg), commitId(commitId), parent(parent), tree(tree) {}

~Commit() {
    // TODO
}    

    void addCommit();
};


class Commands {
    std:: string currentPath;
    bool checkNitDirectory(const std::string& currentPath);

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