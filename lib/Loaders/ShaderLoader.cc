#include "ShaderLoader.h"

using Filesystem::VFS;

Base::Result Loaders::ShaderLoader::LoadObject(Components::Shader &shader, Components::Mesh &mesh, Rendering::RenderSystem &renderSystem){

    shader.meshToLoad = &mesh;

    int loadedId = -1;

    for (auto &loadedShader: renderSystem.loadedShaders) {
        if (loadedShader.first == shader.shaderName)
            loadedId = (int) loadedShader.second;
    }

    if (loadedId == -1) {
        std::string fragBuffer = VFS::ReadToString(shader.mountpoint, (shader.shaderName + ".frag"));
        std::string vertBuffer = VFS::ReadToString(shader.mountpoint, (shader.shaderName + ".vert"));


        const char *vertStr = vertBuffer.c_str();
        const char *fragStr = fragBuffer.c_str();

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertStr, nullptr);
        glCompileShader(vertexShader);

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragStr, nullptr);
        glCompileShader(fragmentShader);


        shader.ID = glCreateProgram();

        glAttachShader(shader.ID, vertexShader);
        glAttachShader(shader.ID, fragmentShader);
        glLinkProgram(shader.ID);

        GLint success;

        glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);

        if (!success) {
            std::vector<GLchar> errorLog(512);
            glGetProgramInfoLog(shader.ID, 512, nullptr, &errorLog[0]);
            MLOG(LOG_ERROR, "\n" + std::string(&errorLog[0]));
        }

        glUseProgram(shader.ID);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        renderSystem.loadedShaders.emplace_back(shader.shaderName, shader.ID);
    } else {
        shader.ID = loadedId;
    }

    renderSystem.ParseCamera(shader.ID);


    return Base::Result::STATUS_OK;
}