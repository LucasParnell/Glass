#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Components/Shader.h"
#include "Components/IComponent.h"
#include "Components/Texture.h"
#include "Components/Mesh.h"
#include "Components/Camera.h"
#include "Loaders/MeshLoader.h"
#include "Base/Window.h"

#include "Managers/ComponentManager.h"
#include "Base/Utils.h"
#include "Filesystem/Vfs.h"
#include "Managers/EntityManager.h"
#include "Loaders/ShaderLoader.h"

namespace Loaders {
    class ShaderLoader;
}

namespace Managers {
    class EntityManager;
}

namespace Rendering {
    class RenderSystem {
    public:
        GLASS_EXPORT RenderSystem();



        void Render(Base::Window *pWindow, Managers::EntityManager *pEntityManager, Managers::ComponentManager *pComponentManager);

        void SetWindow(Base::Window *window);



    private:
        std::vector<std::pair<std::string, uint32_t>> loadedShaders;
        Components::Mesh *mesh;
        Base::Window *window;

        friend class Loaders::ShaderLoader;

        void ParseCamera(uint32_t shaderID);

        //Drawing code
        //TODO(Rewrite most of this)
        static void DrawMesh(const std::map<int, GLuint> &vbos,
                             tinygltf::Model &model, tinygltf::Mesh &mesh);

        static void DrawModel(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos,
                              tinygltf::Model &model);

        static void DrawModelNodes(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos,
                                   tinygltf::Model &model, tinygltf::Node &node);


        static void SetMat4(const std::string &uniform, glm::mat4 matrix, uint32_t shaderID);


    };
}