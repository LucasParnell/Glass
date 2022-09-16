#pragma once
#include "glad/glad.h"
#include "Shader.h"
#include "Mesh.h"
#include <string>

#include "MeshLoader.h"
#include "IComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
struct Renderer {
	__declspec(dllexport) Shader __cdecl LoadObject(Mesh* meshToLoad, std::string shaderName);
	void Render();



private:
	Mesh* mesh;

	//doesn't need to be static
	static std::vector <std::pair<std::string, uint32_t>> loadedShaders;
	
	void ParseCamera(uint32_t shaderID);

	//Cleaner to have small functions here
	void SetMat4(std::string uniform, glm::mat4 matrix, int shaderID) {
		auto location = glGetUniformLocation(shaderID, uniform.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}
	//RegisterShader()
};

#include "Globals.h"