#include "MeshLoader.h"

Mesh MeshLoader::CreateMesh(std::string meshDir) {
	std::fstream fs;
	std::string test;
	fs.open("CoolWorld/box.obj", std::fstream::in);

	fs >> test;

	MLOG(LOG_INFO, test);

	return Mesh();
	
}