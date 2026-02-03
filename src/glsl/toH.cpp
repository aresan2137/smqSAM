#include "glsl.h"

std::string Glsl::glsl_toH() {
	std::string r = "";

	r += "\nenum Materials : unsigned int {\n";

	for (int i = 0; i < shaders.size(); i++) {
		r += std::format("\t{},\n", shaders[i].name);
	}

	r += "};\n\nenum MaterialUniforms : unsigned int {\n";

    for (int i = 0; i < shaders.size(); i++) {
        std::string vsC = "resources/assets/" + shaders[i].VSpath;
        std::vector<Unitform> vsUni = GetUniforms(smq::LoadFile(vsC));

        std::string fsC = "resources/assets/" + shaders[i].FSpath;
        std::vector<Unitform> fsUni = GetUniforms(smq::LoadFile(fsC));

        for (int j = 0; j < vsUni.size(); j++) {
			r += std::format("\t{}_vs_{} = {},\n", shaders[i].name, vsUni[j].name,j);
        }

        for (int j = 0; j < fsUni.size(); j++) {
            r += std::format("\t{}_fs_{} = {},\n", shaders[i].name, fsUni[j].name, vsUni.size() + j);
        }
    }
  
	r += "};\n\nsmq::Material* GetMaterial(Materials material);\n";

	return r;
}