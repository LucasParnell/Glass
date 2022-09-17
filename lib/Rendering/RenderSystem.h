#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Components/IComponent.h"
#include "Components/Texture.h"
#include "Components/Mesh.h"
#include "Components/Camera.h"
#include "Loaders/MeshLoader.h"
#include "Base/Window.h"
#include "Managers/EntityManager.h"
#include "Managers/ComponentManager.h"




class RenderSystem {
 public:
    GLASS_EXPORT RenderSystem();

    GLASS_EXPORT Shader LoadObject(Mesh *meshToLoad, std::string shaderName);

    void Render(Window *window, EntityManager *entityManager, ComponentManager *componentManager);

 private:
    Mesh *mesh;
    Window *window;

    //doesn't need to be static
    static std::vector<std::pair<std::string, uint32_t>> loadedShaders;

    void ParseCamera(uint32_t shaderID);

    //Cleaner to have small functions here
    void SetMat4(std::string uniform, glm::mat4 matrix, int shaderID) {
        auto location = glGetUniformLocation(shaderID, uniform.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    }


};