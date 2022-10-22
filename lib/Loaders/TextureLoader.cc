#include "TextureLoader.h"

using Loaders::TextureLoader;

Base::Result _cdecl TextureLoader::teCreateTexture(Components::Texture &texture, const std::string& mountpoint, const std::string& filename) {

    if(!mountedTextures.contains(mountpoint + filename)){
        std::vector<unsigned char> buffer;
        unsigned width, height;

        auto pfe = Filesystem::VFS::Load(mountpoint, filename);

        unsigned error = lodepng::decode(buffer, width, height, pfe.buffer, pfe.size);

        uint32_t iTexture;
        if (!error){
        glGenTextures(1, &iTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iTexture);
        // set the texture wrapping/filtering options (on the currently bound texture object
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buffer[0]);
        } else {
        MLOG(LOG_ERROR, "Error loading texture: " + std::string(lodepng_error_text(error)));
        return
        Base::Result::STATUS_ERROR;
        }

        texture.ID = iTexture;
        mountedTextures[mountpoint+filename] = texture.ID;

    }
    else
    {
        texture.ID = mountedTextures[mountpoint+filename];
    }

    texture.loaded = true;
    return Base::Result::STATUS_OK;
}