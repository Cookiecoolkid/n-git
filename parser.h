#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <map>
#include <vector>
#include <apis.h>
#include "func.h"
#include <iostream>

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

#endif