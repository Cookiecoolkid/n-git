#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <string>
#include <map>
#include <vector>
#include <apis.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "func.h"
#include "commit.h"
#include "blob.h"

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
    void add(const std::string& filename);
    void checkout(const std::string& commitId);
    void log();
    void commit(const std::string& msg);
    void rm(const std::string& filename);
};


#endif 