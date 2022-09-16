#include "MeshLoader.h"
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
Mesh MeshLoader::CreateMesh(std::string meshDir) {

    std::string inputfile = "CoolWorld/" + meshDir + ".glb";

    auto baseMesh = new Mesh();

    tinygltf::Model model;

    loadModel(model, inputfile.c_str());

    std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos = bindModel(model);

    baseMesh->vaoAndEbos = vaoAndEbos;
    baseMesh->model = model;

    return *baseMesh;
    
}


bool MeshLoader::loadModel(tinygltf::Model& model, const char* filename) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << filename << std::endl;
    else
        std::cout << "Loaded glTF: " << filename << std::endl;

    return res;
}

void MeshLoader::bindMesh(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh) {
    for (size_t i = 0; i < model.bufferViews.size(); ++i) {
        const tinygltf::BufferView& bufferView = model.bufferViews[i];
        if (bufferView.target == 0) {
            MLOG(LOG_WARN, "bufferView.target is zero");
            continue; 

        }

        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
        std::cout << "bufferview.target " << bufferView.target << std::endl;

        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos[i] = vbo;
        glBindBuffer(bufferView.target, vbo);

        std::cout << "buffer.data.size = " << buffer.data.size()
            << ", bufferview.byteOffset = " << bufferView.byteOffset
            << std::endl;

        glBufferData(bufferView.target, bufferView.byteLength,
            &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        for (auto& attrib : primitive.attributes) {
            tinygltf::Accessor accessor = model.accessors[attrib.second];
            int byteStride =
                accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0) vaa = 0;
            if (attrib.first.compare("NORMAL") == 0) vaa = 1;
            if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
            if (vaa > -1) {
                glEnableVertexAttribArray(vaa);
                glVertexAttribPointer(vaa, size, accessor.componentType,
                    accessor.normalized ? GL_TRUE : GL_FALSE,
                    byteStride, BUFFER_OFFSET(accessor.byteOffset));
            }
            else
                std::cout << "vaa missing: " << attrib.first << std::endl;
        }
    }
}

// bind models
void MeshLoader::bindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model,
    tinygltf::Node& node) {
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        bindMesh(vbos, model, model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
        bindModelNodes(vbos, model, model.nodes[node.children[i]]);
    }
}

std::pair<GLuint, std::map<int, GLuint>> MeshLoader::bindModel(tinygltf::Model& model) {
    std::map<int, GLuint> vbos;
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
        bindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
    }


    glBindVertexArray(0);
    // cleanup vbos but do not delete index buffers yet
    for (auto it = vbos.cbegin(); it != vbos.cend();) {
        tinygltf::BufferView bufferView = model.bufferViews[it->first];
        if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
            glDeleteBuffers(1, &vbos[it->first]);
            vbos.erase(it++);
        }
        else {
            ++it;
        }
    }

    return { vao, vbos };
}

//void drawMesh(const std::map<int, GLuint>& vbos,
//    tinygltf::Model& model, tinygltf::Mesh& mesh) {
//    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
//        tinygltf::Primitive primitive = mesh.primitives[i];
//        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));
//
//        glDrawElements(primitive.mode, indexAccessor.count,
//            indexAccessor.componentType,
//            BUFFER_OFFSET(indexAccessor.byteOffset));
//    }
//}

//void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
//    tinygltf::Model& model, tinygltf::Node& node) {
//    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
//        drawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
//    }
//    for (size_t i = 0; i < node.children.size(); i++) {
//        drawModelNodes(vaoAndEbos, model, model.nodes[node.children[i]]);
//    }
//}
//void drawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
//    tinygltf::Model& model) {
//    glBindVertexArray(vaoAndEbos.first);
//
//    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
//    for (size_t i = 0; i < scene.nodes.size(); ++i) {
//        drawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
//    }
//
//    glBindVertexArray(0);
//}

void dbgModel(tinygltf::Model& model) {
    for (auto& mesh : model.meshes) {
        std::cout << "mesh : " << mesh.name << std::endl;
        for (auto& primitive : mesh.primitives) {
            const tinygltf::Accessor& indexAccessor =
                model.accessors[primitive.indices];

            std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
                << indexAccessor.componentType << std::endl;

            tinygltf::Material& mat = model.materials[primitive.material];
            for (auto& mats : mat.values) {
                std::cout << "mat : " << mats.first.c_str() << std::endl;
            }

            for (auto& image : model.images) {
                std::cout << "image name : " << image.uri << std::endl;
                std::cout << "  size : " << image.image.size() << std::endl;
                std::cout << "  w/h : " << image.width << "/" << image.height
                    << std::endl;
            }

            std::cout << "indices : " << primitive.indices << std::endl;
            std::cout << "mode     : "
                << "(" << primitive.mode << ")" << std::endl;

            for (auto& attrib : primitive.attributes) {
                std::cout << "attribute : " << attrib.first.c_str() << std::endl;
            }
        }
    }
}


