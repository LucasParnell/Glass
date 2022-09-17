#pragma once
#include <vector>
#include "IComponent.h"
#include "glad/glad.h"
#include "tinygltf/tiny_gltf.h"
#include "glm/gtc/matrix_transform.hpp"

struct Mesh : IComponent { //Make this easier to use
    Mesh() { componentType = MESH; }

    tinygltf::Model model;
    std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos;
};


