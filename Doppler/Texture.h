#pragma once
#include <string>
#include "glad/glad.h"
#include "lodepng.h"

#include "IComponent.h"
struct Texture : IComponent {
	Texture() {
		this->componentType = TEXTURE;
	}

	bool loaded = false;
	GLuint texture;

	__declspec(dllexport) int _cdecl Load(std::string dir);



};