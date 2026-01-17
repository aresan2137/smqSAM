#include <iostream>

#include "include.h"


Config ParseConfig(const std::string& conf) {
    Config cfg;

    int pos = 0;
    while (pos < conf.size()) {

        int end = conf.find('\n', pos);
        if (end == std::string::npos)
            end = conf.size();

        std::string line = conf.substr(pos, end - pos);
        pos = end + 1;

        if (line.empty() || line[0] == '#')
            continue;

        int eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if (key == "glsl_type")
            cfg.glsl_type = val;
        else if (key == "glsl_ver")
            cfg.glsl_ver = std::stoi(val);
    }

    return cfg;
}

std::vector<ShaderProgram> ParseShader(const std::string& conf) {
    std::vector<ShaderProgram> out;

    size_t pos = 0;
    while (pos < conf.size()) {

        while (pos < conf.size() && std::isspace((unsigned char)conf[pos]))
            pos++;

        if (pos >= conf.size())
            break;

        if (conf[pos] == '#') {
            while (pos < conf.size() && conf[pos] != '\n')
                pos++;
            continue;
        }

        size_t name_start = pos;
        while (pos < conf.size() && !std::isspace((unsigned char)conf[pos]))
            pos++;

        std::string name = conf.substr(name_start, pos - name_start);

        while (pos < conf.size() && std::isspace((unsigned char)conf[pos]))
            pos++;

        size_t vs_start = pos;
        while (pos < conf.size() && !std::isspace((unsigned char)conf[pos]))
            pos++;

        std::string vs = conf.substr(vs_start, pos - vs_start);

        while (pos < conf.size() && std::isspace((unsigned char)conf[pos]))
            pos++;

        size_t fs_start = pos;
        while (pos < conf.size() && !std::isspace((unsigned char)conf[pos]))
            pos++;

        std::string fs = conf.substr(fs_start, pos - fs_start);

        if (!vs.empty() && !fs.empty())
            out.push_back({ name, vs, fs });
    }

    return out;
}

std::vector<SmfConf> ParseSmf(const std::string& conf) {
    std::vector<SmfConf> out;

    size_t pos = 0;
    while (pos < conf.size()) {

        while (pos < conf.size() && std::isspace((unsigned char)conf[pos]))
            pos++;

        if (pos >= conf.size())
            break;

        if (conf[pos] == '#') {
            while (pos < conf.size() && conf[pos] != '\n')
                pos++;
            continue;
        }

        size_t name_start = pos;
        while (pos < conf.size() && !std::isspace((unsigned char)conf[pos]))
            pos++;

        std::string name = conf.substr(name_start, pos - name_start);

        while (pos < conf.size() && std::isspace((unsigned char)conf[pos]))
            pos++;

        size_t fl_start = pos;
        while (pos < conf.size() && !std::isspace((unsigned char)conf[pos]))
            pos++;

        std::string fl = conf.substr(fl_start, pos - fl_start);

        while (pos < conf.size() && std::isspace((unsigned char)conf[pos]))
            pos++;


        if (!fl.empty())
            out.push_back({ name, fl });
    }

    return out;
}

std::string NoniString(const std::string& str) {
    std::string out = str;

    const std::string ext = ".glsl";
    if (out.size() >= ext.size() &&
        out.compare(out.size() - ext.size(), ext.size(), ext) == 0) {
        out.erase(out.size() - ext.size(), ext.size());
    }

    for (char& c : out) {
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            c = '_';
        }
    }

    return out;
}

int main() {
    Config config = ParseConfig(smq::LoadFile("resources/config.conf"));
	std::vector<ShaderProgram> shaders = ParseShader(smq::LoadFile("resources/glsl.conf"));
    std::vector<SmfConf> smf = ParseSmf(smq::LoadFile("resources/smf.conf"));
    std::vector<SmfConf> image = ParseSmf(smq::LoadFile("resources/img.conf"));
	MakeCpp(config, shaders, smf, image);
	MakeH(config, shaders, smf, image);
}

