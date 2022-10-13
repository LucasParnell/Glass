
#include "Vfs.h"
#include "Base/Logging.h"
#include <sys/stat.h>

Base::Result VFS::Mount(std::string filename) {
    FILE* inFile = fopen(filename.c_str(), "rb");

    char fileTableBuffer[SIZE_FILETABLE];

    struct stat sb;
    if (stat(filename.c_str(), &sb) == -1) {

        MLOG(LOG_ERROR, "File Error");
        return Base::Result::STATUS_ERROR;
    }

    //TODO(Check File Header for 'GPAK')

    MLOG(LOG_INFO, "Mounting VFS: '" + filename +"'");
    for(int i=0; i<SIZE_FILETABLE; i+=4){
        //Read Offset

        char * c_gpkName = (char*)malloc(8);

        fseek(inFile, 5, SEEK_SET);
        fread(c_gpkName, 8, 1, inFile);

        auto * offset = (long*)malloc(4);

        fseek(inFile, OFFSET_FILETABLE + i, SEEK_SET);
        fread(offset, 4, 1, inFile);

        //If the offset is 0, we have reached the last offset and can continue
        if(!*offset)
            break;

        //Read File Header
        char* header = (char*)malloc(SIZE_FILEHEADER);

        fseek(inFile, *offset, SEEK_SET);
        fread(header, SIZE_FILEHEADER, 1, inFile);

        //Get filename
        char* c_fileName = (char*)malloc(32);
        memcpy(c_fileName, header+128, 32);



        //Get mountpoint
        char* c_mountpoint = (char*)malloc(128);
        memcpy(c_mountpoint, header, 128);
        std::string sMountpoint = std::string(c_mountpoint);
        std::string sFileName = std::string(c_fileName);
        std::string sGpkName = std::string(c_gpkName);

        Base::Utils::stringReplace(sMountpoint, "root\\", sGpkName + ":\\");


        //If mountpoint is not yet in the map, store it, along with a pointer to its file header
        //Then, Add current file to the mountpoint
        if(!mountMap.contains(sMountpoint)){
            mountMap[sMountpoint] = std::vector<FileEntry>();
        }

        mountMap[sMountpoint].emplace_back(FileEntry{sFileName, *offset});
        MLOG(LOG_INFO, "Mounted, mountpoint: '" + sMountpoint +"'");
    }
    fclose(inFile);




    return Base::Result::STATUS_OK;
}

Base::Result GLASS_EXPORT VFS::ListAll() {
    for (auto const& [key, val] : mountMap)
    {
        std::string entries;
        entries += "\n";
        entries += "Mountpoint: " + key + "\n";

        for(const auto& entry : val){
            entries += entry.name + "\n";
        }
        MLOG(LOG_INFO, entries);
    }
    return Base::Result::STATUS_OK;
}

Base::Result GLASS_EXPORT VFS::ReadToString(std::string *string, std::string filename) {

    //Split filename by / delimiter

    auto mountpoint = filename.substr(0, Base::Utils::strpos(const_cast<char *>(filename.c_str()),
                                                             reinterpret_cast<char *>('\\'), 2));
    MLOG(LOG_INFO, "Mountpoint is: " + mountpoint);

    return Base::Result::STATUS_WARN;
}
