#include "Texture.h" 

#include "Logging.h"

int Texture::Load(std::string dir) {
 std::vector<unsigned char> buffer;
 unsigned width, height;

 unsigned error = lodepng::decode(buffer, width, height, dir);


 if (!error) {
	 glGenTextures(1, &texture);
	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, texture);
	 // set the texture wrapping/filtering options (on the currently bound texture object)
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buffer[0]);

	 loaded = true;
 }
 else {
	 MLOG(LOG_ERROR, "Error loading texture: " + std::string(lodepng_error_text(error)));
 }

 return 0;
	}
