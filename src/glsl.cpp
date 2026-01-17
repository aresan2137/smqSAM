#include "include.h"

std::vector<Unitform> GetUniforms(std::string src);
std::string UniformToNString(std::string type);

std::string glslComp(Config config, std::vector<ShaderProgram> shaders) {
	std::string h = "";

    std::vector<Shader> uniqeShaders;

    for (int i = 0; i < shaders.size(); i++) {
        std::string& vs = shaders[i].vs;
        bool vsE = false;
        for (int j = 0; j < uniqeShaders.size(); j++) {
            if (vs == uniqeShaders[j].name) {
                vsE = true;
            }
        }
        if (!vsE) uniqeShaders.push_back({ shaders[i].vs, "GL_VERTEX_SHADER" });
        std::string& fs = shaders[i].fs;
        bool fsE = false;
        for (int j = 0; j < uniqeShaders.size(); j++) {
            if (fs == uniqeShaders[j].name) {
                fsE = true;
            }
        }
        if (!fsE) uniqeShaders.push_back({ shaders[i].fs, "GL_FRAGMENT_SHADER" });
    }

    for (int i = 0; i < uniqeShaders.size(); i++) {
        h += "\nunsigned int ";
        h += NoniString(uniqeShaders[i].name);
        h += ";\n";
        h += "CompileShaderProgram(";
        h += NoniString(uniqeShaders[i].name);
        h += ",R\"(\n";
        h += "#version ";
        h += std::to_string(config.glsl_ver);
        h += " ";
        h += config.glsl_type;
        h += "\n";
        if (config.glsl_type == "es") {
            h += "precision mediump float;\n";
        }
        std::string sh = "resources/assets/" + uniqeShaders[i].name;
        h += smq::LoadFile(sh);
        h += ")\", ";
        h += uniqeShaders[i].type;
        h += ");\n";
    }
    h += "\nshaders.resize(";
    h += std::to_string(shaders.size());
    h += ");\n";

    for (int i = 0; i < shaders.size(); i++) {
        h += "\nshaders[";
        h += std::to_string(i);
        h += "] = Shader(";
        h += NoniString(shaders[i].vs);
        h += ", ";
        h += NoniString(shaders[i].fs);
        h += ");\n";
    }

    for (int i = 0; i < uniqeShaders.size(); i++) {
        h += "\nglDeleteShader(";
        h += NoniString(uniqeShaders[i].name);
        h += ");";
    }
    h += "\n\n";

    h += "materials.reserve(";
    h += std::to_string(shaders.size());
    h += ");\n";

    for (int i = 0; i < shaders.size(); i++) {

        std::vector<Unitform> vsUni = GetUniforms(smq::LoadFile("resources/assets/" + shaders[i].vs));
        std::vector<Unitform> fsUni = GetUniforms(smq::LoadFile("resources/assets/" + shaders[i].fs));

        h += "\nstd::vector<unsigned int> uniforms_";
        h += shaders[i].name;
        h += ";\n";
        h += "uniforms_";
        h += shaders[i].name;
        h += ".resize(";
        h += std::to_string(vsUni.size() + fsUni.size());
        h += ");\n";

        h += "\nstd::vector<smq::GlAtribute> atributes_";
        h += shaders[i].name;
        h += ";\n";
        h += "atributes_";
        h += shaders[i].name;
        h += ".resize(";
        h += std::to_string(vsUni.size() + fsUni.size());
        h += ");\n";

        for (int j = 0; j < vsUni.size(); j++) {
            h += "uniforms_";
            h += shaders[i].name;
            h += "[";
            h += std::to_string(j);
            h += "] = glGetUniformLocation(shaders[";
            h += std::to_string(i);
            h += "],\"";
            h += vsUni[j].name;
            h += "\");\n";

            h += "atributes_";
            h += shaders[i].name;
            h += "[";
            h += std::to_string(j);
            h += "].type = smq::GlUniform_";
            std::string val = UniformToNString(vsUni[j].type);
            h += val;
            h += ";\n";

            h += "atributes_";
            h += shaders[i].name;
            h += "[";
            h += std::to_string(j);
            h += "].";


            if (val == "Float") {
                h += "f = 0.0f;\n";
            }

            if (val == "Vec2") {
                h += "f2 = {0.0f,0.0f};\n";
            }

            if (val == "Vec3") {
                h += "f3 = {0.0f,0.0f,0,0f};\n";
            }

            if (val == "Vec4") {
                h += "f4 = {0.0f,0.0f,0.0f,0.0f};\n";
            }

            if (val == "Int") {
                h += "i = 0;\n";
            }

            if (val == "Ivec2") {
                h += "i2 = {0,0};\n";
            }

            if (val == "Ivec3") {
                h += "i3 = {0,0,0};\n";
            }

            if (val == "Ivec4") {
                h += "i4 = {0,0,0,0};\n";
            }

            if (val == "Mat4") {
                h += "m4 = {0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f};\n";
            }
        }
        for (int j = 0; j < fsUni.size(); j++) {
            h += "uniforms_";
            h += shaders[i].name;
            h += "[";
            h += std::to_string(vsUni.size() + j);
            h += "] = glGetUniformLocation(shaders[";
            h += std::to_string(i);
            h += "],\"";
            h += fsUni[j].name;
            h += "\");\n";


            h += "atributes_";
            h += shaders[i].name;
            h += "[";
            h += std::to_string(vsUni.size() + j);
            h += "].type = smq::GlUniform_";
            std::string val = UniformToNString(fsUni[j].type);
            h += val;
            h += ";\n";

            h += "atributes_";
            h += shaders[i].name;
            h += "[";
            h += std::to_string(vsUni.size() + j);
            h += "].";


            if (val == "Float") {
                h += "f = 0.0f;\n";
            }

            if (val == "Vec2") {
                h += "f2 = {0.0f,0.0f};\n";
            }

            if (val == "Vec3") {
                h += "f3 = {0.0f,0.0f,0,0f};\n";
            }

            if (val == "Vec4") {
                h += "f4 = {0.0f,0.0f,0.0f,0,0f};\n";
            }

            if (val == "Int") {
                h += "i = 0;\n";
            }

            if (val == "Ivec2") {
                h += "i2 = {0,0};\n";
            }

            if (val == "Ivec3") {
                h += "i3 = {0,0,0};\n";
            }

            if (val == "Ivec4") {
                h += "i4 = {0,0,0,0};\n";
            }

            if (val == "Mat4") {
                h += "m4 = {0.0f,0.0f,0.0f,0,0f, 0.0f,0.0f,0.0f,0,0f, 0.0f,0.0f,0.0f,0,0f, 0.0f,0.0f,0.0f,0,0f};\n";
            }
        }


        h += "\nmaterials.push_back(smq::Material(shaders[";
        h += std::to_string(i);
        h += "], ";
        h += "uniforms_";
        h += shaders[i].name;
        h += ", atributes_";
        h += shaders[i].name;
        h += "));\n";
    }

    return h;
}