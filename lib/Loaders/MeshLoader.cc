#include "MeshLoader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using Loaders::MeshLoader;

Base::Result
MeshLoader::meCreateMesh(Components::Mesh &baseMesh, const std::string &mountpoint, const std::string &filename) {


    tinygltf::Model model;

    bLoadModel(model, mountpoint, filename);

    std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos = bindModel(model);

    baseMesh.vaoAndEbos = vaoAndEbos;
    baseMesh.model = model;

    return Base::Result::STATUS_OK;

}

bool MeshLoader::bLoadModel(tinygltf::Model &model, const std::string &mountpoint, const std::string &filename) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    //Filename is mountpoint
    auto a = Filesystem::VFS::Load(mountpoint, filename);


    bool res = loader.LoadBinaryFromMemory(&model, &err, &warn, a.buffer, a.size);

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << filename << std::endl;


    return res;
}

//TODO(REWRITE THIS, VERY TEMPORARY)
void MeshLoader::bindMesh(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh) {
    for (size_t i = 0; i < model.bufferViews.size(); ++i) {
        const tinygltf::BufferView &bufferView = model.bufferViews[i];
        if (bufferView.target == 0) {
            MLOG(LOG_WARN, "bufferView.target is zero");
            continue;

        }

        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos[i] = vbo;
        glBindBuffer(bufferView.target, vbo);

        glBufferData(bufferView.target, bufferView.byteLength,
                     &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (auto primitive: mesh.primitives) {
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        for (auto &attrib: primitive.attributes) {
            tinygltf::Accessor accessor = model.accessors[attrib.second];
            int byteStride =
                    accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first == "POSITION") vaa = 0;
            if (attrib.first == "NORMAL") vaa = 1;
            if (attrib.first == "TEXCOORD_0") vaa = 2;
            if (vaa > -1) {
                glEnableVertexAttribArray(vaa);
                glVertexAttribPointer(vaa, size, accessor.componentType,
                                      accessor.normalized ? GL_TRUE : GL_FALSE,
                                      byteStride, BUFFER_OFFSET(accessor.byteOffset));
            } else
                std::cout << "vaa missing: " << attrib.first << std::endl;
        }
    }
}

// bind models
//TODO(REWRITE THIS, RECURSION IS BAD)
void MeshLoader::bindModelNodes(std::map<int, GLuint> &vbos, tinygltf::Model &model,
                                tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        bindMesh(vbos, model, model.meshes[node.mesh]);
    }

    for (int i: node.children) {
        assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
        bindModelNodes(vbos, model, model.nodes[i]);
    }
}

std::pair<GLuint, std::map<int, GLuint>> MeshLoader::bindModel(tinygltf::Model &model) {
    std::map<int, GLuint> vbos;
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const tinygltf::Scene &scene = model.scenes[model.defaultScene];
    for (int node: scene.nodes) {
        bindModelNodes(vbos, model, model.nodes[node]);
    }


    glBindVertexArray(0);
    // cleanup vbos but do not delete index buffers yet
    for (auto it = vbos.cbegin(); it != vbos.cend();) {
        tinygltf::BufferView bufferView = model.bufferViews[it->first];
        if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
            glDeleteBuffers(1, &vbos[it->first]);
            vbos.erase(it++);
        } else {
            ++it;
        }
    }

    return {vao, vbos};
}