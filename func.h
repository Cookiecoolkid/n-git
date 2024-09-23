#ifndef __FUNC_H__
#define __FUNC_H__

#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <fstream>
#include <apis.h>

#define DEBUG(x) std::cerr << #x << " " << x << std::endl

namespace fs = std::filesystem;


class Functions {
public: 
    std::string processContentByLine(const std::string& srcContent, std::string& dstContent);
    std::string getCurrentCommitTreeContent();
    std::string getNitContent(const std::string& filename);
    std::string extractContent(const std::string& content, const std::string& start, const std::string& end);
    std::string extractContentPlus(const std::string& content, const std::string& start, const std::string& end);
    void eraseSubstrFromContent(const std::string& substr, std::string& content, const std::string& contentPath);
    void removeLineContainingSubstringFromContent(std::string& content, const std::string& substr, const std::string& contentPath);
    void removeFileAndEmptyDirectory(const std::string& dir, const std::string& filename);
    void removeFilenameFromContent(std::string& content, const std::string& contentPath);
    void clearIndexRemove();
    void deleteFileIfExists(const std::string& path);

};

#endif