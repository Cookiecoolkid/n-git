#include "func.h"
#include <iostream>
#include <filesystem>
#include "apis.h"

namespace fs = std::filesystem;


bool Commands:: checkNitDirectory() {
    fs::path path(currentPath);
    fs::path nitDirectory = path / ".nit";

    return fs::exists(nitDirectory) && fs::is_directory(nitDirectory);
}

void Commands::checkInit() {
    if (!checkNitDirectory()) {
        std::cerr << "Not a Nit repository" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Commands::init() {
    if (checkNitDirectory()) {
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
    
    const std::string initMsg = "initial commit";
    commit(initMsg);
    
}

void Commands::status() {
      // TODO
    checkInit();
}

void Commands::add() {
      // TODO
    checkInit();
}

void Commands::checkout() {
      // TODO
    checkInit();
}

void Commands::log() {
      // TODO
    checkInit();
}

void Commands::commit(const std::string& msg) { 
    checkInit();
    // Assume that the file object has created successfully by "nit add" command
    // Check file "HEAD" to tell whether the commit is the initial commit
    // Initall commit's parent is nullptr
    // Build Tree by reading the file "index"
        
    std::string headPath = UsefulApi::cwd() + "/.nit/HEAD";
    std::string headContent;
    std::string indexPath = UsefulApi::cwd() + "/.nit/index";
    std::string indexContent;

    // Create tree object
    Tree * tree;
    std::string treeSha1;
    std::string treeContent;
    std::string treePath;

    std::string parentSha1    = "";
    std::string commitContent = "";
    std::string commitSha1    = "";
    std::string commitPath    = "";
    
    std::ostringstream commitContentStream;


    if (!UsefulApi::readFromFile(indexPath, indexContent) || 
        !UsefulApi::readFromFile(headPath, headContent)) {
                        
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    if (!headContent.empty() && indexContent.empty()) {
        std::cerr << "No changes added to the commit." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // treeContent: Sha1 filename\n Sha1 filename\n...
    // if tree is empty tree, then this must be the initial commit
    treeContent = indexContent;
    treeSha1    = UsefulApi::hash(indexContent);
    treePath    = UsefulApi::cwd() + "/.nit/objects/" + treeSha1.substr(0, 2) + "/" + treeSha1.substr(2);

    tree = new Tree(treeSha1, treeContent, treePath);

    // Create commit object: Msg Tree Parent CommitSha1
    // parentSha1 = "" when it is the initial commit
    parentSha1 = headContent;

    commitContentStream << "tree:" << treeSha1 << "\n";
    if (!parentSha1.empty()) {
        commitContentStream << "parent:" << parentSha1 << "\n";
    }
    commitContentStream << msg << "\n";

    commitContent = commitContentStream.str();
    commitSha1    = UsefulApi::hash(commitContent);
    commitPath    = UsefulApi::cwd() + "/.nit/objects/" + commitSha1.substr(0, 2) + "/" + commitSha1.substr(2);

    commitCommand = new Commit(msg, commitContent, commitSha1, commitPath, parentSha1, tree);
    commitCommand->writeTreeCommitLogHEAD();

    // TODO
    delete tree;
}

void Commit::writeTreeCommitLogHEAD() {
    std::string treePath = tree->getTreePath();
    std::string logPath  = UsefulApi::cwd() + "/.nit/log";
    std::string headPath = UsefulApi::cwd() + "/.nit/HEAD";

    std::string treeContent = tree->getTreeContent();
    std::string logContent = "===\ncommit " + commitSha1 + "\n" + msg + "\n";
    std::string logContent_old;

    if (!UsefulApi::readFromFile(logPath, logContent_old)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    logContent = logContent + logContent_old;
    
    if (!UsefulApi::writeToFile(treeContent, treePath) ||
        !UsefulApi::writeToFile(commitContent, commitPath) ||
        !UsefulApi::writeToFile(logContent, logPath) ||
        !UsefulApi::writeToFile(commitSha1, headPath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "commit: "  << this->msg << std::endl << "OK!" << std::endl;
}

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
        commands.status();
    } else if (args[1] == "add") {
        if (args.size() != 3) printUsageAndExit();
        commands.add();
    } else if (args[1] == "checkout") {
        if (args.size() != 3) printUsageAndExit();
        commands.checkout();
    } else if (args[1] == "log") {
        if (args.size() != 2) printUsageAndExit();
        commands.log();
    } else if (args[1] == "commit") {
        if (args.size() == 2) std::cerr << "please enter a commit message." << std::endl;
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
