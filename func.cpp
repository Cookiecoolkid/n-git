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
    
    // TIP: sha1 = hash(filename + "\n" + fileContent)
    std::string filenameCatContent = filename + "\n" + fileContent;

    fileSha1 = UsefulApi::hash(filenameCatContent);

    Blob blob(filenameCatContent, fileSha1, filename);

    blob.checkBlobState();
    Blob::BlobStatus status = blob.getStatus();

    if (status == Blob::BlobStatus::ADD) {
        blob.writeBlobAndIndex();
    } else if (status == Blob::BlobStatus::MODIFIED_ADDED) {
        // Delete the old file blob & index
        // Write the new file blob & index

        // find the sha1 by filename in indexContent
        // TIP blobSha1_old can be found in first line, which can't match "\n"
        std::string blobSha1_old = functions.extractContentPlus(indexContent, "\n", " " + filename);
        std::string blobFilename_old = blobSha1_old.substr(2);
        std::string blobDir_old  = UsefulApi::cwd() + "/.nit/objects/" + blobSha1_old.substr(0, 2);

        functions.removeFileAndEmptyDirectory(blobDir_old, blobFilename_old);
        functions.removeLineContainingSubstringFromContent(indexContent, filename, indexPath);
        blob.writeBlobAndIndex();

    } else if (status == Blob::BlobStatus::MODIFIED_COMMITED) {
          // Delete the old file in index if it exists
          // Write the new file blob & index
        if (indexContent.find(filename) != std::string::npos) {
            functions.removeLineContainingSubstringFromContent(indexContent, filename, indexPath);
        }

        blob.writeBlobAndIndex();

    } else if (status == Blob::BlobStatus::SAME) {
        if (indexContent.find(filename) != std::string::npos) {
            functions.removeLineContainingSubstringFromContent(indexContent, filename, indexPath);
        }

    } else if (status == Blob::BlobStatus::REMOVED) {
        functions.removeLineContainingSubstringFromContent(indexContent, filename, indexPath);

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
    
    Functions functions;
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
    
      // if headContent is empty, then getCurrentCommitTreeContent() will return ""
      // Commit Inherit the parent's tree  
      
    std::string currentCommitTreeContent            = functions.getCurrentCommitTreeContent();
    std::string currentCommitTreeContentExceptIndex = functions.processContentByLine(indexContent, currentCommitTreeContent);
    

    treeContent = indexContent + currentCommitTreeContentExceptIndex;
    treeSha1    = UsefulApi::hash(indexContent);
    treePath    = UsefulApi::cwd() + "/.nit/objects/" + treeSha1.substr(0, 2) + "/" + treeSha1.substr(2);

    tree = new Tree(treeSha1, treeContent, treePath);

    // Create commit object: Msg Tree Parent CommitSha1
    // parentSha1 = "" when it is the initial commit
    parentSha1 = headContent;

    commitContentStream << "tree:" << treeSha1 << "\n";
    commitContentStream << "parent:" << parentSha1 << "\n";
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

    if (!UsefulApi::readFromFile(indexPath, indexContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (!UsefulApi::readFromFile(headPath, headContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // Can't read content means file NOTEXIST or REMOVED
    if (!UsefulApi::readFromFile(filePath, fileContent)) {
        std::string substr = filename;
        size_t      found  = indexContent.find(substr);

        if (found != std::string::npos) {
            status = BlobStatus::REMOVED;
            DEBUG("Got REMOVED");
        } else {
            status = BlobStatus::NOTEXIST;
            DEBUG("Got NOTEXIST");
        }
        return;
    }
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

        // Same as current commit
        if (currentCommitTreeContent.find(sha1 + " " + filename) != std::string::npos) {
            status = BlobStatus::SAME;
            DEBUG("Got SAME");
            return;
        }

        // Check "MODIFIED_COMMITED" status: This file is modified in the current commit
        if (currentCommitTreeContent.find(filename) != std::string::npos && 
            currentCommitTreeContent.find(sha1) == std::string::npos) {
            status = BlobStatus::MODIFIED_COMMITED;
            DEBUG("Got MODIFIED_COMMITED");
            return;
        }
    }
    // TIP: Same filename with <Same content> will recongnize as "MODIFIED"
    // Check "MODIFIED_ADDED" status: This file isn't in the current commit
    if (indexContent.find(filename) != std::string::npos) {
        status = BlobStatus::MODIFIED_ADDED;
        DEBUG("Got MODIFIED_ADDED");
        return;
    }
    status = BlobStatus::ADD;
    DEBUG("Got ADD");
}

void Blob::writeBlobAndIndex() {
    std::string indexPath = UsefulApi::cwd() + "/.nit/index";  

    // Write the new file blob & index
    // TIP: sha1 = hash(filename + "\n" + fileContent)
    
    std::string blobPath     = UsefulApi::cwd() + "/.nit/objects/" + sha1.substr(0, 2) + "/" + sha1.substr(2);
    std::string indexContent = sha1 + " " + filename + "\n";
    std::string indexContent_old;

    if (!UsefulApi::readFromFile(indexPath, indexContent_old)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    indexContent = indexContent + indexContent_old;

    if (!UsefulApi::writeToFile(content, blobPath)) {   
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

std::string Functions:: getCurrentCommitTreeContent() {
    std::string headPath = UsefulApi::cwd() + "/.nit/HEAD";
    std::string headContent;
    std::string currentCommitPath;
    std::string currentCommitContent;
    std::string currentCommitTreePath;
    std::string currentCommitTreeContent;

    if (!UsefulApi::readFromFile(headPath, headContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (headContent.empty()) {
        return "";
    }

    currentCommitPath = UsefulApi::cwd() + "/.nit/objects/" + headContent.substr(0, 2) + "/" + headContent.substr(2);

    if (!UsefulApi::readFromFile(currentCommitPath, currentCommitContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string treeSha1 = extractContent(currentCommitContent, "tree:", "\n");
    currentCommitTreePath = UsefulApi::cwd() + "/.nit/objects/" + treeSha1.substr(0, 2) + "/" + treeSha1.substr(2);

    if (!UsefulApi::readFromFile(currentCommitTreePath, currentCommitTreeContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return currentCommitTreeContent;
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


void Functions::removeLineContainingSubstringFromContent(std::string& content, const std::string& substr,
    const std::string& contentPath) {
    std::istringstream stream(content);
    std::ostringstream result;
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find(substr) == std::string::npos) {
            result << line << '\n';
        }
    }
    content = result.str();

    if (contentPath != "" && !UsefulApi::writeToFile(content, contentPath)) {
        std::cerr << "Failed to write file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
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
  /**
 * @brief Process the content line by line. Delete the line containing the filename in dstContent.
 */

std::string Functions::processContentByLine(const std::string& srcContent, std::string& dstContent) {
    std::istringstream srcStream(srcContent);
    std::string line;
    std::string filename;

    while (std::getline(srcStream, line)) {
        std::istringstream lineStream(line);
        std::string sha1;
        lineStream >> sha1 >> filename;

        if (dstContent.find(filename) != std::string::npos) {
            removeLineContainingSubstringFromContent(dstContent, filename, "");
        }
    }
    return dstContent;
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
