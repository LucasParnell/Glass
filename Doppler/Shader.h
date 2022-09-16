#pragma once
#include <cstdint>
#include <vector>
#include "IComponent.h"
#include "Transform.h"
struct Shader : public IComponent {
	Shader() {
		componentType = SHADER;
	}	
	uint32_t ID;
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;

};