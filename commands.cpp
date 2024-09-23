#include "commands.h"

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
        UsefulApi::writeToFile("", ".nit/remove");
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

    std::cout << "=== Staged Files ===" << std::endl;
    Functions functions;
    std::string indexContent  = functions.getNitContent("index");
    std::string removeContent = functions.getNitContent("remove");
    
    std::istringstream indexStream(indexContent);
    std::string line;
    while (std::getline(indexStream, line)) {
        std::istringstream lineStream(line);
        std::string sha1;
        std::string filename;
        lineStream >> sha1 >> filename;
        std::cout << filename << std::endl;
    }

    std::cout << std::endl;

    std::cout << "=== Removed Files ===" << std::endl;
    std::istringstream removeStream(removeContent);
    while (std::getline(removeStream, line)) {
        std::cout << line << std::endl;
    }
}

void Commands::add(const std::string& filename) {
    // TODO
    checkInit();
      // add: Create file blob object and write to .nit/objects
      // update file "index" after checking the file got already added/modified/removed or not
    Functions functions;
    std::string filePath = UsefulApi::cwd() + "/" + filename;
    std::string fileContent;
    std::string fileSha1;
    std::string indexPath    = UsefulApi::cwd() + "/.nit/index";
    std::string indexContent = functions.getNitContent("index");


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
    Functions functions;
    std::string logContent = functions.getNitContent("log");
    std::cout << logContent << std::endl;
}

void Commands::commit(const std::string& msg) { 
    checkInit();
    // Assume that the file object has created successfully by "nit add" command
    // Check file "HEAD" to tell whether the commit is the initial commit
    // Initall commit's parent is nullptr
    // Build Tree by reading the file "index"
    
    Functions functions;
    std::string headPath     = UsefulApi::cwd() + "/.nit/HEAD";
    std::string headContent  = functions.getNitContent("HEAD");
    std::string indexPath    = UsefulApi::cwd() + "/.nit/index";
    std::string indexContent = functions.getNitContent("index");
    std::string removeContent = functions.getNitContent("remove");

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
    
    if (!headContent.empty() && indexContent.empty() && removeContent.empty()) {
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
      // Remove file By reading the file "remove"
    std::string removePath = UsefulApi::cwd() + "/.nit/remove";
    functions.removeFilenameFromContent(treeContent, removePath); 

    treeSha1    = UsefulApi::hash(treeContent);
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

void Commands::rm(const std::string& filename) {
    // TODO
    checkInit();
    Functions functions;
    std::string removePath        = UsefulApi::cwd() + "/.nit/remove";
    std::string removeContent_old = functions.getNitContent("remove");
    std::string indexPath         = UsefulApi::cwd() + "/.nit/index";
    std::string indexContent      = functions.getNitContent("index");
    std::string currentCommitTreeContent = functions.getCurrentCommitTreeContent();
    std::string removeContent;

    

    if (indexContent.find(filename) != std::string::npos || 
        currentCommitTreeContent.find(filename) != std::string::npos) {
        removeContent = filename + "\n" + removeContent_old;
        if (!UsefulApi::writeToFile(removeContent, removePath)) {
            std::cerr << "Failed to write file" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        functions.removeLineContainingSubstringFromContent(indexContent, filename, indexPath);
    } else {
        std::cerr << "File does not exist." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /* 
    // if the file just added, but not commited ever, remove .nit/objects/xx/xx 
    std::string indexContent = functions.getNitContent("index");
    if (indexContent.find(filename) != std::string::npos) {
        std::string blobSha1 = functions.extractContentPlus(indexContent, "\n", " " + filename);
        std::string blobFilename = blobSha1.substr(2);
        std::string blobDir = UsefulApi::cwd() + "/.nit/objects/" + blobSha1.substr(0, 2);

            // TODO or FIXME 
            // OK. I thought we don't need to remove the file, leave it there!
            // If we decide to remove it, should check the file is not in the history commit
            // And remove "Dead commit" files when checkout
          
        functions.removeFileAndEmptyDirectory(blobDir, blobFilename);
    } else {
        std::cerr << "File does not exist." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    */
}