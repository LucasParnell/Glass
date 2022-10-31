#include "Shader.h"

using Components::Shader;

REGISTER_DEF_TYPE(Shader);

Base::Result Components::Shader::SetMembers(std::vector<std::string> list) {
    //some error checking
    if (list.size() != 2) {
        MLOG(LOG_ERROR, "Size of shader array in GSD file is invalid.");
        return Base::Result::STATUS_ERROR;
    }


    this->mountpoint = list[0];
    this->shaderName = list[1];


    return Base::Result::STATUS_WARN;
}
