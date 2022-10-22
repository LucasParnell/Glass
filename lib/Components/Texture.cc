#include "Texture.h"
#include "Base/Logging.h"
#include "Loaders/TextureLoader.h"
using Components::Texture;

REGISTER_DEF_TYPE(Texture);

Base::Result Texture::SetMembers(std::vector<std::string> list) {

    //some error checking
    if(list.size()!=2){
        MLOG(LOG_ERROR, "Size of texture array in GSD file is invalid.");
        return Base::Result::STATUS_ERROR;
    }

    Loaders::TextureLoader::teCreateTexture(*this, list[0], list[1]);



    return Base::Result::STATUS_OK;
}
