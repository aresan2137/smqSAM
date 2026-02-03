#include "img.h"

std::string Img::img_toH() {
	std::string r = "";

	r += "\nenum Textures : unsigned int {\n";

	for (int i = 0; i < images.size(); i++) {
		r += std::format("\t{},\n", images[i].name);
	}

	r += "};\n\nsmq::Texture* GetTexture(Textures texture);\n";


	return r;
}