#include "Include.h"




/*
void MakeH(Config config, std::vector<ShaderProgram> shaders, std::vector<SmfConf> smf, std::vector<SmfConf> image) {
    std::string h = R"(
#pragma once

enum ShaderUniform : unsigned int {
)";

    for (int i = 0; i < shaders.size(); i++) {
        std::string vsC = "resources/assets/" + shaders[i].vs;
		std::vector<Unitform> vsUni = GetUniforms(smq::LoadFile(vsC));

        std::string fsC = "resources/assets/" + shaders[i].fs;
        std::vector<Unitform> fsUni = GetUniforms(smq::LoadFile(fsC));

        for (int j = 0; j < vsUni.size(); j++) {
            h += NoniString(shaders[i].name);
            h += "_vs_";
            h += vsUni[j].name;
            h += " = ";
			h += std::to_string(j);
            h +=",\n";
        }

        for (int j = 0; j < fsUni.size(); j++) {
            h += NoniString(shaders[i].name);
            h += "_fs_";
            h += fsUni[j].name;
            h += " = ";
            h += std::to_string(vsUni.size() + j);
            h += ",\n";
        }        
	}

    h += R"(
};

enum Shaders {
)";

    for (int i = 0; i < shaders.size(); i++) {
        h += shaders[i].name;
        h += ",\n";
    }

    h += R"(
};

enum Meshes {
)";

    for (int i = 0; i < smf.size(); i++) {
        h += smf[i].name;
        h += ",\n";
    }

    h += R"(
};

enum Textures {
)";

    for (int i = 0; i < image.size(); i++) {
        h += image[i].name;
        h += ",\n";
    }

    h += R"(
};

namespace smq {
	class Material;
    class Mesh;
    class Texture;
}

void initSam();
smq::Material* getMaterial(Shaders shader);
smq::Mesh* getMesh(Meshes mesh);
smq::Texture* getTexture(Textures texture);

)";

    std::string put = config.output + "/sam.h";
    smq::SaveFile(put, h);
}
*/