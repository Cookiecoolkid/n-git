#include "func.h"
#include <iostream>
#include <filesystem>
#include "apis.h"



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

std::string Functions:: getNitContent(const std::string& filename) {
    std::string path = UsefulApi::cwd() + "/.nit/" + filename;
    std::string content;

    if (!UsefulApi::readFromFile(path, content)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return content;
}

std::string Functions:: getCurrentCommitTreeContent() {
    Functions functions;
    std::string headContent = functions.getNitContent("HEAD");
    std::string currentCommitPath;
    std::string currentCommitContent;
    std::string currentCommitTreePath;
    std::string currentCommitTreeContent;

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

void Functions:: removeFilenameFromContent(std::string& content, const std::string& contentPath) {
    std::string srcContent;
    if (!UsefulApi::readFromFile(contentPath, srcContent)) {
        std::cerr << "Failed to read file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::istringstream srcStream(srcContent);
    std::string filename;
    while (std::getline(srcStream, filename)) {
        removeLineContainingSubstringFromContent(content, filename, "");
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

void Functions::deleteFileIfExists(const std::string& path) {
    try {
        if (fs::exists(path)) {
            fs::remove(path);
        } 
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}


void Functions:: clearIndexRemove() {
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