#include "func.h"
#include <iostream>
#include <filesystem>
#include "apis.h"

namespace fs = std::filesystem;

#define DEBUG(x) std::cerr << #x << " " << x << std::endl

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

void Commands::add(const std::string& filename) {
      // TODO
    checkInit();
      // add: Create file blob object and write to .nit/objects
      // update file "index" after checking the file got already added/modified/removed or not
    
    std::string filePath = UsefulApi::cwd() + "/" + filename;
    std::string fileContent;
    std::string fileSha1;
    std::string indexPath = UsefulApi::cwd() + "/.nit/index";
    std::string indexContent;
    Functions functions;

    if (!UsefulApi::readFromFile(indexPath, indexContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!UsefulApi::readFromFile(filePath, fileContent)) {
        std::cerr << "Try to add a removed file or file not exist" << std::endl;
        fileContent = "";
    }
    
    // TIP
    std::string filenameCatContent = filename + "\n" + fileContent;

    fileSha1 = UsefulApi::hash(filenameCatContent);

    Blob blob(fileContent, fileSha1, filename);

    blob.checkBlobState();
    Blob::BlobStatus status = blob.getStatus();


    if (status == Blob::BlobStatus::ADD || status == Blob::BlobStatus::MODIFIED) {
        blob.writeBlobAndIndex(status);
    } else if (status == Blob::BlobStatus::SAME || status == Blob::BlobStatus::REMOVED) {
        //FIXME : doesn't work to remove successfully
        functions.removeLineContainingSubstringFromContent(indexContent, filename);
        std::string blobSha1_old = functions.extractContentPlus(indexContent, "\n", " " + filename);
        // FIXME : out of range for substr
        std::string blobFilename_old = blobSha1_old.substr(2);
        std::string blobDir_old = UsefulApi::cwd() + "/.nit/objects/" + blobSha1_old.substr(0, 2);

        functions.removeFileAndEmptyDirectory(blobDir_old, blobFilename_old);
    } else if (status == Blob::BlobStatus::NOTEXIST) {
        std::cerr << "File does not exist." << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        std::cerr << "Add fail: Unchecked or unknown status." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Commands::checkout() {
      // TODO
    checkInit();
}

void Commands::log() {
    checkInit();
    std::string logPath = UsefulApi::cwd() + "/.nit/log";
    std::string logContent;

    if (!UsefulApi::readFromFile(logPath, logContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << logContent << std::endl;
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

    // FIXME： Should I delete here?
    delete tree;
}

void Commit::writeTreeCommitLogHEAD() {
    std::string treePath = tree->getTreePath();
    std::string logPath  = UsefulApi::cwd() + "/.nit/log";
    std::string headPath = UsefulApi::cwd() + "/.nit/HEAD";
    std::string indexPath = UsefulApi::cwd() + "/.nit/index";

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

    // clear "index"
    if (!UsefulApi::writeToFile("", indexPath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
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

std::string Blob::getContent() const {
    return content;
}

std::string Blob::getSha1() const {
    return sha1;
}

Blob::BlobStatus Blob::getStatus() const {
    return status;
}

void Blob::checkBlobState() {
    Functions functions;

    std::string filePath = UsefulApi::cwd() + "/" + filename;
    std::string fileContent;

    std::string indexPath = UsefulApi::cwd() + "/.nit/index";
    std::string indexContent;

    std::string headPath = UsefulApi::cwd() + "/.nit/HEAD";
    std::string headContent;

    if (!UsefulApi::readFromFile(indexPath, indexContent) && 
        !UsefulApi::readFromFile(headPath, headContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }


    // Can't read content means file NOTEXIST or REMOVED
    if (!UsefulApi::readFromFile(filePath, fileContent)) {
        std::string substr = filename;
        size_t      found  = indexContent.find(substr);

        if (found != std::string::npos) {
            status = BlobStatus::REMOVED;
        } else {
            status = BlobStatus::NOTEXIST;
        }
        return;
    }

    // FIXME haven't check the FUNCTION "SAME"
    // Check "SAME" status: This file is the same as that in current commit
    if (!headContent.empty()){
        std::string currentCommitPath = UsefulApi::cwd() + "/.nit/objects/" + headContent.substr(0, 2) + "/" + headContent.substr(2);
        std::string currentCommitContent;
        std::string currentCommitTreePath;
        std::string currentCommitTreeContent;

        if (!UsefulApi::readFromFile(currentCommitPath, currentCommitContent)) {
            std::cerr << "Failed to read file" << std::endl;
            std::exit(EXIT_FAILURE);
        }


        std::string treeSha1 = functions.extractContent(currentCommitContent, "tree:", "\n");
        currentCommitTreePath = UsefulApi::cwd() + "/.nit/objects/" + treeSha1.substr(0, 2) + "/" + treeSha1.substr(2);
        
        if (!UsefulApi::readFromFile(currentCommitTreePath, currentCommitTreeContent)) {
            std::cerr << "Failed to read file" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (currentCommitTreeContent.find(sha1) != std::string::npos) {
            status = BlobStatus::SAME;
            return;
        }
    }
          // Check "ADD" & "MODIFIED" status: This file is added in the current commit
          // By comparing the filename
    if (indexContent.find(filename) == std::string::npos) {
        status = BlobStatus::ADD;
    } else {
        status = BlobStatus::MODIFIED;
    }
}

void Blob::writeBlobAndIndex(BlobStatus status) {
    //TODO
    // MODIFIED: Delete the old file blob & index and then write the new file & index
    // ADD: Write the new file & index


    std::string indexPath = UsefulApi::cwd() + "/.nit/index";  
    Functions functions;
        // Delete the old file blob & index
    if (status == BlobStatus::MODIFIED) {
        std::string indexContent;

        if (!UsefulApi::readFromFile(indexPath, indexContent)) {
            std::cerr << "Failed to read file" << std::endl;
            std::exit(EXIT_FAILURE);
        }
       
            // find the sha1 by filename in indexContent
            // TIP blobSha1_old can be found in first line, which can't match "\n"
        std::string blobSha1_old = functions.extractContentPlus(indexContent, "\n", " " + filename);

        DEBUG(blobSha1_old);

        std::string blobFilename_old = blobSha1_old.substr(2);
        std::string blobDir_old  = UsefulApi::cwd() + "/.nit/objects/" + blobSha1_old.substr(0, 2);

        functions.removeFileAndEmptyDirectory(blobDir_old, blobFilename_old);

        functions.eraseSubstrFromContent(blobSha1_old + " " + filename + "\n", indexContent, indexPath);

        DEBUG(indexContent);
    }


          // Write the new file blob & index
    std::string blobFilenameCatContent = filename + "\n" + content;
    std::string blobPath     = UsefulApi::cwd() + "/.nit/objects/" + sha1.substr(0, 2) + "/" + sha1.substr(2);
    std::string indexContent = sha1 + " " + filename + "\n";
    std::string indexContent_old;

    if (!UsefulApi::readFromFile(indexPath, indexContent_old)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    indexContent = indexContent + indexContent_old;

    if (!UsefulApi::writeToFile(blobFilenameCatContent, blobPath)) {   
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!UsefulApi::writeToFile(indexContent, indexPath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

}

  /**
 * @brief If [filename, sha1] exists in the index, erase it.
 */
void Functions::eraseSubstrFromContent(const std::string& substr, std::string& content, 
    const std::string& contentPath) {
    size_t      found  = content.find(substr);
    if (found != std::string::npos) {
        content.erase(found, substr.length());
    }

    if (!UsefulApi::writeToFile(content, contentPath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

std::string Functions:: extractContent(const std::string& content, const std::string& start, const std::string& end) {
    size_t startPos = content.find(start);
    size_t endPos = content.find(end, startPos + start.length());
    if (startPos == std::string::npos || endPos == std::string::npos) {
        return "";
    }
    return content.substr(startPos + start.length(), endPos - startPos - start.length());
}

std::string Functions::extractContentPlus(const std::string& content, const std::string& start, const std::string& end) {
    std::string substr = extractContent(content, start, end);

    if (substr.empty()) {
        substr = extractContent(content, "", end);
    }
    return substr;
}

void Functions::removeLineContainingSubstringFromContent
    (std::string& content, const std::string& substr) {
    std::istringstream stream(content);
    std::ostringstream result;
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find(substr) == std::string::npos) {
            result << line << '\n';
        }
    }
    content = result.str();   
}

void Functions::removeFileAndEmptyDirectory(const std::string& dir, const std::string& filename) {
    fs::path path(dir);
    fs::path file = path / filename;

    if (!fs::remove(file)) {
        std::cerr << "Failed to remove file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (fs::is_empty(dir)) {
        if (!fs::remove(dir)) {
            std::cerr << "Failed to remove directory: " << dir << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
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
        commands.add(args[2]);
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
