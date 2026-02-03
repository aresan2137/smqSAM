#include "include.h"



/*
void MakeCpp(Config config, std::vector<ShaderProgram> shaders, std::vector<SmfConf> smf, std::vector<SmfConf> image) {
    std::string h = R"(
#include "sam.h"

#include <Include.h>

)";
    h += "smq::Material* materials[";
    h += std::to_string(shaders.size());
    h += "];\n";

    h += "smq::Mesh* meshes[";
    h += std::to_string(smf.size());
    h += "];\n";

    h += "smq::Texture* textures[";
    h += std::to_string(image.size());
    h += "];\n";

    h += R"(

void CompileShaderProgram(unsigned int& program, const char* Code, unsigned int type) {
    program = glCreateShader(type);
    glShaderSource(program, 1, &Code, nullptr);
    glCompileShader(program);

    int ok;
    glGetShaderiv(program, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(program, 1024, nullptr, log);
        smq::Error(log);
    }
}


unsigned int Shader(unsigned int vs,unsigned int fs) {
    unsigned int i_shader = glCreateProgram();

    glAttachShader(i_shader, vs);
    glAttachShader(i_shader, fs);
    glLinkProgram(i_shader);
    return i_shader;
}

smq::Material* getMaterial(Shaders shader) {
	return materials[static_cast<int>(shader)];
}

smq::Mesh* getMesh(Meshes mesh) {
	return meshes[static_cast<int>(mesh)];
}

smq::Texture* getTexture(Textures texture) {
	return textures[static_cast<int>(texture)];
}

void initSam() {
    )";

    //std::string glsl = glslComp(config, shaders);
    //h += glsl;
    h += "\n";
    //std::string models = smfComp(config, smf);

    //h += models;  
    h += "\n";

    for (int i = 0; i < image.size(); i++) {
        h += "textures[";
        h += std::to_string(i);
        h += "] = new smq::Texture(\"resources/textures/";
        h += image[i].fl;
        h += "\");\n";
    }

    h += "\n}";

    std::string put = config.output + (std::string)"sam.cpp";

    smq::SaveFile(put, h);
}
*/