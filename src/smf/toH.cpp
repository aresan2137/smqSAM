#include "smf.h"

std::string Smf::smf_toH() {
	std::string r = "";

	r += "enum Meshes : unsigned int {\n";

	for (int i = 0; i < models.size(); i++) {
		r += std::format("\t{},\n", models[i].name);
	}

	r += "};\n\nsmq::Mesh* GetMesh(Meshes mesh);\n";

	return r;
}