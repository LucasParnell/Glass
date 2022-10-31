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

namespace Filesystem {

    struct FileEntry {
        std::string name;
        std::string base_file;
        long offset;
        long size;
    };

    struct PopulatedFileEntry : public FileEntry {
        unsigned char *buffer{};
    };


    class VFS {
    public:
        enum FileMode {
            Read,
            Write
        };

        static PopulatedFileEntry GLASS_EXPORT Load(std::string mountpoint, std::string filename);

        static Base::Result GLASS_EXPORT Mount(const std::string &filename);

        static Base::Result GLASS_EXPORT ListAll();

        static std::string GLASS_EXPORT ReadToString(std::string mountpoint, std::string filename);

        static Base::Result GLASS_EXPORT Unload(PopulatedFileEntry *pfe);


    private:
        inline static std::unordered_map<std::string, std::vector<FileEntry>> mountMap;


    };

}