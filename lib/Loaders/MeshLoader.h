#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>

#include "glass_export.h"
#include "glad/glad.h"

#include "Components/Mesh.h"
#include "Debug/Logging.h"





class MeshLoader {

public:
    GLASS_EXPORT Mesh meCreateMesh(const char *meshDir);

private:
    bool bLoadModel(tinygltf::Model &model, const char *filename);

    void bindModelNodes(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node);

    std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model &model);

    void bindMesh(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);
};