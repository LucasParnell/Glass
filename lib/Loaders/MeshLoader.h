#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>

#include "glass_export.h"
#include "glad/glad.h"

#include "Filesystem/Vfs.h"

#include "Components/Mesh.h"
#include "Base/Logging.h"

namespace Loaders {
    class MeshLoader {
    public:
        GLASS_EXPORT Base::Result
        meCreateMesh(Components::Mesh &mesh, const std::string &mountpoint, const std::string &filename);

    private:
        static bool bLoadModel(tinygltf::Model &model, const std::string &mountpoint, const std::string &filename);

        void bindModelNodes(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node);

        std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model &model);

        static void bindMesh(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);
    };
}