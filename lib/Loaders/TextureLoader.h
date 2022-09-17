#pragma once

#include "Components/Texture.h"
#include "Debug/Logging.h"
#include "Base/Window.h"

class TextureLoader {
public:
    __declspec(dllexport) Texture _cdecl teCreateTexture(const char *texDir);


};

