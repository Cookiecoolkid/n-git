#ifndef __BLOB_H__
#define __BLOB_H__

#include <string>
#include <map>
#include <vector>
#include <apis.h>
#include "func.h"
#include <iostream>

class Blob {
    std::string content;
    std::string sha1;
    std::string filename;
public:
    enum class BlobStatus {
        UNCHECKED,
        SAME,
        ADD,
        MODIFIED_ADDED,
        MODIFIED_COMMITED,
        REMOVED,
        NOTEXIST
    };
private:
    BlobStatus status = BlobStatus::UNCHECKED;

public                                                   : 
Blob(const std::string& content, const std::string& sha1, const std::string& filename)
    : content(content), sha1(sha1), filename(filename) {}
    void writeBlobAndIndex();
    std::string getContent() const;
    std::string getSha1() const;
    BlobStatus getStatus() const;

    void checkBlobState();
};


#endif