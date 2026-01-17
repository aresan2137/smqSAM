#include <string>
#include <vector>

#include "File.h"
#include "Log.h"

struct Config {
    std::string glsl_type;
    int glsl_ver;
};

struct ShaderProgram {
	std::string name;
    std::string vs;
    std::string fs;
};

struct Shader {
    std::string name;
    std::string type;
};

struct Unitform {
    std::string name;
	std::string type;
};

struct SmfConf {
    std::string name;
    std::string fl;
};

std::string NoniString(const std::string& str);

void MakeH(Config config, std::vector<ShaderProgram> shaders, std::vector<SmfConf> smf, std::vector<SmfConf> image);
void MakeCpp(Config config, std::vector<ShaderProgram> shaders, std::vector<SmfConf> smf, std::vector<SmfConf> image);
std::string glslComp(Config config, std::vector<ShaderProgram> shaders);
std::string smfComp(Config config, std::vector<SmfConf> smf);