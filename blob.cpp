#include "blob.h"

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

    std::string indexContent = functions.getNitContent("index");
    std::string headContent = functions.getNitContent("HEAD");
    
    std::string content = functions.getCurrentCommitTreeContent();

    // Can't read content means file NOTEXIST or REMOVED
    if (!UsefulApi::readFromFile(filePath, fileContent)) {
        std::string substr = filename;

        if (indexContent.find(substr) != std::string::npos || content.find(filename) != std::string::npos) {
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
        std::string currentCommitTreeContent = functions.getCurrentCommitTreeContent();

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
    Functions functions;
    std::string indexPath  = UsefulApi::cwd() + "/.nit/index";
    std::string removePath = UsefulApi::cwd() + "/.nit/remove";
    std::string removeContent = functions.getNitContent("remove");
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

    functions.removeLineContainingSubstringFromContent(removeContent, filename, removePath);
}