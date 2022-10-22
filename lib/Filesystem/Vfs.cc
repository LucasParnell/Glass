
#include "Vfs.h"
#include "Base/Logging.h"
#include <sys/stat.h>

using Filesystem::VFS;

Base::Result VFS::Mount(const std::string& base_file) {
    FILE* inFile = fopen(base_file.c_str(), "rb");

    char fileTableBuffer[SIZE_FILETABLE];

    struct stat sb;
    if (stat(base_file.c_str(), &sb) == -1) {

        MLOG(LOG_ERROR, "File Error");
        return Base::Result::STATUS_ERROR;
    }

    //TODO(Check File Header for 'GPAK')

    MLOG(LOG_INFO, "Mounting VFS: '" + base_file +"'");
    for(int i=0; i<SIZE_FILETABLE; i+=4){
        //Read Offset

        //Name
        char * c_gpkName = (char*)malloc(8);

        fseek(inFile, 5, SEEK_SET);
        fread(c_gpkName, 8, 1, inFile);

        //File Table Offset
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

        //Get mountpoint
        char* c_mountpoint = (char*)malloc(128);
        memcpy(c_mountpoint, header, 128);

        //Get filename
        char* c_fileName = (char*)malloc(32);
        memcpy(c_fileName, header+128, 32);

        //Get file size
        long* c_size = (long*)malloc(4);
        memcpy(c_size, header+0xA1, 4);


        std::string sMountpoint = std::string(c_mountpoint);

        std::string sFileName = std::string(c_fileName);

        std::string sGpkName = std::string(c_gpkName);
        Base::Utils::stringReplace(sMountpoint, "root\\", sGpkName + ":\\");


        //If mountpoint is not yet in the map, store it, along with a pointer to its file header
        //Then, Add current file to the mountpoint
        if(!mountMap.contains(sMountpoint)){
            mountMap[sMountpoint] = std::vector<FileEntry>();
        }

        mountMap[sMountpoint].emplace_back(FileEntry{sFileName, base_file,*offset, *c_size});
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

std::string GLASS_EXPORT VFS::ReadToString(std::string mountpoint, std::string filename) {
    //Load file and cast to const char*
    PopulatedFileEntry pfe = VFS::Load(mountpoint, filename);
    std::string buffer(reinterpret_cast<const char*>(pfe.buffer), pfe.size);

    return buffer;
}

Base::Result Filesystem::VFS::Unload(Filesystem::PopulatedFileEntry* pfe){
    free(pfe->buffer);
    delete pfe;
    return Base::Result::STATUS_OK;
}

Filesystem::PopulatedFileEntry Filesystem::VFS::Load(std::string mountpoint, std::string filename) {
    {
        //Load file entry into memory (Array of primitive T)

        if (mountMap.contains(mountpoint)) {

            auto filesInDir = mountMap[mountpoint];
            FileEntry targetFile;

            for (const auto &file: filesInDir)
                if (file.name == filename)
                    targetFile = file;


            FILE *inFile = fopen(targetFile.base_file.c_str(), "rb");

            unsigned char *file_contents = (unsigned char *) malloc(targetFile.size);


            fseek(inFile, targetFile.offset + 0xA5, SEEK_SET); //File Header is 0xA5
            fread(file_contents, targetFile.size, 1, inFile);
            fclose(inFile);

            PopulatedFileEntry pfe = {targetFile.name, targetFile.base_file, targetFile.offset, targetFile.size,
                                      file_contents};

            return pfe;

        } else {
            MLOG(LOG_ERROR, "Could not find file " + filename + " at mountpoint " + mountpoint);
            return PopulatedFileEntry{};
        }
    }
}