#include <execution>
#include "Transform.h"
#include "Base/Logging.h"

using Components::Transform;

REGISTER_DEF_TYPE(Transform);

Base::Result Components::Transform::SetMembers(std::vector<std::string> list) {
    //Transform members
    std::vector<float> transformVec(list.size());
    std::transform(list.begin(), list.end(), transformVec.begin(), [](const std::string& val)
    {
        return std::stof(val);
    });

    //some error checking
    if(transformVec.size()!=3){
        MLOG(LOG_ERROR, "Size of transform array in GSD file is invalid.");
        return Base::Result::STATUS_ERROR;
    }

    //Convert to glm::vec3
    this->position.x = transformVec[0];
    this->position.y = transformVec[1];
    this->position.z = transformVec[2];
    return Base::Result::STATUS_OK;
}
