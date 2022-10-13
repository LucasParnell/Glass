#pragma once
#include "Base/Result.h"
#include "glass_export.h"
#include "Base/Utils.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

#define OFFSET_FILETABLE 0x11
#define SIZE_FILETABLE 0x1000
#define SIZE_FILEHEADER 0xA5

struct FileEntry {
    std::string name;
    long offset;
};

class VFS {
 public:
    enum FileMode {
        Read,
        Write
    };
    static Base::Result GLASS_EXPORT Mount(std::string filename);
    static Base::Result GLASS_EXPORT ListAll();
    static Base::Result GLASS_EXPORT ReadToString(std::string* string, std::string filename);
 private:
    inline static std::unordered_map<std::string, std::vector<FileEntry>> mountMap;
};

