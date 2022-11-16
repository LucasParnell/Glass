#include "Mesh.h"
#include "Debug/Logging.h"
#include "Loaders/MeshLoader.h"

using Components::Mesh;

REGISTER_DEF_TYPE(Mesh);

Base::Result Components::Mesh::SetMembers(std::vector<std::string> list) {
    //some error checking
    if (list.size() != 2) {
        MLOG(LOG_ERROR, "Size of mesh array in GSD file is invalid.");
        return Base::Result::STATUS_ERROR;
    }

    //TODO(MAKE STATIC)
    Loaders::MeshLoader ml;
    ml.meCreateMesh(*this, list[0], list[1]);

    return Base::Result::STATUS_OK;
}
