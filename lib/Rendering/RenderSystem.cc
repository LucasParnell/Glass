#include "RenderSystem.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace Rendering;

RenderSystem::RenderSystem() = default;

void RenderSystem::SetWindow(Base::Window *window){
    this->window = window;
}

Components::Shader RenderSystem::LoadObject(Components::Mesh *meshToLoad, std::string shaderName) {

    mesh = meshToLoad;

    auto shader = new Components::Shader();

    int loadedId = -1;

    for (auto &loadedShader: loadedShaders) {
        if (loadedShader.first == shaderName)
            loadedId = (int) loadedShader.second;
    }

    if (loadedId == -1) {
        std::ifstream frag("CoolWorld/" + shaderName + ".frag");
        std::stringstream fragBuffer;
        fragBuffer << frag.rdbuf();

        //TODO(Split filename into mountpoint and directory)
        std::string outputBuffer;
        VFS::ReadToString(&outputBuffer, "GamePak:\\textures\\rock.png");

        std::ifstream vert("CoolWorld/" + shaderName + ".vert");
        std::stringstream vertBuffer;
        vertBuffer << vert.rdbuf();

        auto a = vertBuffer.str();
        auto b = fragBuffer.str();

        const char *vertStr = a.c_str();
        const char *fragStr = b.c_str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertStr, nullptr);
        glCompileShader(vertexShader);

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragStr, nullptr);
        glCompileShader(fragmentShader);


        shader->ID = glCreateProgram();

        glAttachShader(shader->ID, vertexShader);
        glAttachShader(shader->ID, fragmentShader);
        glLinkProgram(shader->ID);

        GLint success;

        glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);

        if (!success) {
            std::vector<GLchar> errorLog(512);
            glGetProgramInfoLog(shader->ID, 512, nullptr, &errorLog[0]);
            MLOG(LOG_ERROR, "\n" + std::string(&errorLog[0]));
        }

        glUseProgram(shader->ID);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        loadedShaders.emplace_back(shaderName, shader->ID);
    } else {
        shader->ID = loadedId;
    }

    ParseCamera(shader->ID);


    return *shader;
}

void RenderSystem::ParseCamera(uint32_t shaderID) {
    //TODO(ADD FUNCTION TO PASS IN WINDOW)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) (window->width) / (float) (window->height),
                                            0.1f,
                                            100.0f);
    SetMat4("projection", projection, shaderID);

    Components::Camera *mainCam = Components::Camera::pGetMainCamera();
    SetMat4("view", mainCam->m4GetView(), shaderID);


}


void RenderSystem::DrawMesh(const std::map<int, GLuint> &vbos,
                            tinygltf::Model &model, tinygltf::Mesh &mesh) {
    for (const auto &primitive: mesh.primitives) {
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

        glDrawElements(primitive.mode, (int) indexAccessor.count,
                       indexAccessor.componentType,
                       BUFFER_OFFSET(indexAccessor.byteOffset));
    }
}

// recursively draw node and children nodes of model (Change this)
void RenderSystem::DrawModelNodes(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos,
                                  tinygltf::Model &model, tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        DrawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
    }
    for (int i: node.children) {
        RenderSystem::DrawModelNodes(vaoAndEbos, model, model.nodes[i]);
    }
}

void RenderSystem::DrawModel(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos,
                             tinygltf::Model &model) {
    glBindVertexArray(vaoAndEbos.first);

    const tinygltf::Scene &scene = model.scenes[model.defaultScene];
    for (int node: scene.nodes) {
        DrawModelNodes(vaoAndEbos, model, model.nodes[node]);
    }

    glBindVertexArray(0);
}


void RenderSystem::Render(Base::Window *pWindow, Managers::EntityManager *pEntityManager, Managers::ComponentManager *pComponentManager) {
    //Make this better please
    this->window = pWindow;

    for (const Entity &entity: pEntityManager->entities) {
        auto components = &pComponentManager->entityComponents[entity];

        glm::mat4 model = glm::mat4(1.0f);
        Components::Shader *shader = nullptr;
        bool textureLoaded = false;
        GLuint texture;

        for(auto& p_component : pComponentManager->components){
            //Could replace these with integers and use switch
            auto name = p_component->GetTypeInfo().Name();
            switch(Base::Utils::hash(name.c_str(), name.size())){
                case (Base::Utils::hash("Components::Transform", 21)): {
                    auto *transform = (Components::Transform *) p_component;
                    glm::translate(model, transform->position);
                    break;
                }
                case (Base::Utils::hash("Components::Shader", 18)): {
                    shader = (Components::Shader *) p_component;
                    break;
                }
                case (Base::Utils::hash("Components::Texture", 19)): {
                    auto *pTexture = (Components::Texture *) p_component;
                    if (pTexture->loaded) {
                        textureLoaded = true;
                        texture = pTexture->ID;
                    }
                    break;
                }
            }


        }

        if (!shader) {
            continue;
        }


        glUseProgram(shader->ID);

        if (textureLoaded) {
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        ParseCamera(shader->ID);
        SetMat4("model", model, shader->ID);

        DrawModel(mesh->vaoAndEbos, mesh->model);
        glBindVertexArray(0);


    }


}

void RenderSystem::SetMat4(const std::string &uniform, glm::mat4 matrix, uint32_t shaderID) {
    auto location = glGetUniformLocation(shaderID, uniform.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}
