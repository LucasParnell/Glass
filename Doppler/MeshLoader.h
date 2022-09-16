#pragma once
#include "glad/glad.h"
#include "Mesh.h"
#include <string>
#include <cstdint>
#include <fstream>
#include "Logging.h"
#include "tiny_gltf.h"


struct MeshLoader {
	_declspec(dllexport) Mesh __cdecl CreateMesh(std::string meshDir);
private:
	bool loadModel(tinygltf::Model& model, const char* filename);
	void bindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Node& node);
	std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model& model);

	void bindMesh(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh);


};