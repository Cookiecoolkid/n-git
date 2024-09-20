#include "func.h"
#include <iostream>
#include <filesystem>
#include "apis.h"

namespace fs = std::filesystem;


bool Commands:: checkNitDirectory(const std::string& currentPath) {
    fs::path path(currentPath);
    fs::path nitDirectory = path / ".nit";

    return fs::exists(nitDirectory) && fs::is_directory(nitDirectory);
}

void Commands::init() {
    if (checkNitDirectory(currentPath)) {
        std::cerr << "A Nit version-control system already exists in the current directory." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    try {
        fs::create_directory(".nit");
        fs::create_directory(".nit/objects");

        UsefulApi::writeToFile("", ".nit/log");
        UsefulApi::writeToFile("", ".nit/HEAD");
        UsefulApi::writeToFile("", ".nit/index");
    } catch (fs::filesystem_error& e) {
        std::cerr << "Failed to create a Nit version-control system: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string emptyTreeContent = "";
    std::string emptyTreeSha1 = UsefulApi::hash(emptyTreeContent);
    std::string emptyTreePath = ".nit/objects/" + emptyTreeSha1.substr(0, 2) + "/" + emptyTreeSha1.substr(2);
    fs::create_directories(fs::path(emptyTreePath).parent_path());
    UsefulApi::writeToFile(emptyTreeContent, emptyTreePath);

    const std::string initMsg = "initial commit";
    std::ostringstream commitContent;

    commitContent << "tree " << emptyTreeSha1 << "\n\n" << initMsg << "\n";

    std::string commitSha1 = UsefulApi::hash(commitContent.str());
    std::string commitPath = ".nit/objects/" + commitSha1.substr(0, 2) + "/" + commitSha1.substr(2);
    fs::create_directories(fs::path(commitPath).parent_path());
    UsefulApi::writeToFile(commitContent.str(), commitPath);

    // Update HEAD to point to the current commit
    UsefulApi::writeToFile(commitSha1, ".nit/HEAD");

    commit(initMsg);
}

void Commands::status() {
    // TODO
}

void Commands::add() {
    // TODO
}

void Commands::checkout() {
    // TODO
}

void Commands::log() {
    // TODO
}

void Commands::commit(const std::string& msg) { 
      // create a commit object
      // TODO
    std::string commitId = "generated_commit_id";  
    Tree*       tree     = nullptr;                
    Commit*     parent   = commitCommand;          

    commitCommand = new Commit(msg, commitId, parent, tree);
    commitCommand->addCommit();
}

void Commit::addCommit() {
      // TODO
    std::cout << "commit: " << this->msg << std::endl;
}


void Parser::getArgs() {
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
}

void Parser::parse() {
    if (args.size() < 2) printUsageAndExit();

    Commands commands(UsefulApi::cwd());

    if (args[1] == "init") {
        if (args.size() != 2) printUsageAndExit();
        commands.init();
    } else if (args[1] == "status") {
        if (args.size() != 2) printUsageAndExit();
        // status();
    } else if (args[1] == "add") {
        if (args.size() != 3) printUsageAndExit();
        // add();
    } else if (args[1] == "checkout") {
        if (args.size() != 3) printUsageAndExit();
        // checkout();
    } else if (args[1] == "log") {
        if (args.size() != 2) printUsageAndExit();
        // log();
    } else if (args[1] == "commit") {
        if (args.size() != 3) printUsageAndExit();
        commands.commit(args[2]);
    } else {
        printUsageAndExit();
    }
}

void Parser::printUsageAndExit() {
    std::cerr << "Usage: nit <command> [arguments]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Commands:" << std::endl;
    std::cerr << "  init                    Initialize a new Nit repository" << std::endl;
    std::cerr << "  status                  Show the working tree status" << std::endl;
    std::cerr << "  add <file>              Add file contents to the index" << std::endl;
    std::cerr << "  checkout <commit id>    Checkout a commit" << std::endl;
    std::cerr << "  log                     Show commit hash value and msg" << std::endl;
    std::cerr << "  commit <msg>            Record changes to the repository" << std::endl;
    std::exit(EXIT_FAILURE);
}

std::string Blob::getContent() const {
    return content;
}

std::string Blob::getSha1() const {
    return sha1;
}

void Tree::addFile(const std::string& fileName, std::string& sha1) {
    files.push_back(std::make_pair(fileName, sha1));
}