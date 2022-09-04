#pragma once
#include "Mesh.h"
#include <string>
#include <cstdint>
#include <fstream>
#include "Logging.h"
struct MeshLoader {
	Mesh CreateMesh(std::string meshDir);
};