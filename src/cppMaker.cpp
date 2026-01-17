#include "include.h"

std::string UniformToNString(std::string type) {
    if (type == "float")  return "Float";
    if (type == "vec2")   return "Vec2";
    if (type == "vec3")   return "Vec3";
    if (type == "vec4")   return "Vec4";

    if (type == "int")    return "Int";
    if (type == "ivec2")  return "IVec2";
    if (type == "ivec3")  return "IVec3";
    if (type == "ivec4")  return "IVec4";

    if (type == "mat4")   return "Mat4";

    if (type == "sampler2D")
        return "Int";

	smq::Error("Unknown uniform type: " + type);
    return "Unknown";
}


void MakeCpp(Config config, std::vector<ShaderProgram> shaders, std::vector<SmfConf> smf, std::vector<SmfConf> image) {
    std::string h = R"(
#include "../Smq/Include.h"

std::vector<unsigned int> shaders;
std::vector<smq::Material> materials;
std::vector<smq::Mesh> meshes;
std::vector<smq::Texture> textures;

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
	return &materials[static_cast<int>(shader)];
}

smq::Mesh* getMesh(Meshes mesh) {
	return &meshes[static_cast<int>(mesh)];
}

smq::Texture* getTexture(Textures texture) {
	return &textures[static_cast<int>(texture)];
}

void initSam() {
    )";

    std::string glsl = glslComp(config, shaders);
    h += glsl;
    h += "\n";
    std::string models = smfComp(config, smf);

    h += models;  
    h += "\n";

    h += "textures.reserve(";
    h += std::to_string(image.size());
    h += ");\n";
    for (int i = 0; i < image.size(); i++) {
        h += "textures.push_back(smq::Texture(\"resources/textures/";
        h += image[i].fl;
        h += "\"));\n";
    }

    h += "\n}";
    smq::SaveFile("resources/sam.cpp", h);
}