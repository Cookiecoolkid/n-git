#include "commit.h"

void Commit::writeTreeCommitLogHEAD() {
    std::string treePath = tree->getTreePath();
    std::string logPath  = UsefulApi::cwd() + "/.nit/log";
    std::string headPath = UsefulApi::cwd() + "/.nit/HEAD";
    std::string indexPath = UsefulApi::cwd() + "/.nit/index";

    std::string treeContent = tree->getTreeContent();
    std::string logContent = "===\ncommit " + commitSha1 + "\n" + msg + "\n\n";
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

    std::cout << "commit message: "  << this->msg << std::endl << "OK!" << std::endl;

    // clear "index" & "remove" file
    clearIndexRemove();
}

void Commit:: clearIndexRemove() {
    std::string indexPath = UsefulApi::cwd() + "/.nit/index";
    std::string removePath = UsefulApi::cwd() + "/.nit/remove";

    if (!UsefulApi::writeToFile("", indexPath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!UsefulApi::writeToFile("", removePath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
