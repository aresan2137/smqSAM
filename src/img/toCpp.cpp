#include "img.h"

std::string Img::img_toCpp() {
	std::string r = "";


    r += std::format("smq::Texture* img[{}];", images.size());

    r += R"(
smq::Texture* GetTexture(Textures texture) {
    return img[static_cast<unsigned int>(texture)];
}

void Iimg() {
)";

    for (int i = 0; i < images.size(); i++) {
		r += std::format("img[{}] = new smq::Texture(\"resources/{}\");\n", i, images[i].path);
    }

    r += std::format(R"(
}}

void Qimg() {{
    for (int i = 0; i < {}; i++) {{
        delete img[i];
    }}
}}

)", images.size());

	return r;	
}
