#include "Include.h"



std::vector<Unitform> GetUniforms(std::string src) {
    std::vector<Unitform> out;

    int i = 0;
    int n = src.size();

    auto is_ws = [](char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
        };

    auto is_ident = [](char c) {
        return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_';
        };

    while (i < n) {

        if (i + 7 <= n &&
            src.compare(i, 7, "uniform") == 0 &&
            (i == 0 || !is_ident(src[i - 1])) &&
            (i + 7 == n || is_ws(src[i + 7]))) {

            i += 7;

            while (i < n && is_ws(src[i])) i++;
            int type_start = i;
            while (i < n && is_ident(src[i])) i++;
            std::string type = src.substr(type_start, i - type_start);

            while (i < n && is_ws(src[i])) i++;
            int name_start = i;
            while (i < n && is_ident(src[i])) i++;
            std::string name = src.substr(name_start, i - name_start);

            int br = name.find('[');
            if (br != std::string::npos)
                name = name.substr(0, br);

            if (!type.empty() && !name.empty()) {
                out.push_back({ name, type });
            }
        }

        i++;
    }

    return out;
}


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

    smq::SaveFile("resources/sam.h", h);
}