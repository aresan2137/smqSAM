#include "glsl.h"
#include "../control/control.h"

std::string Glsl::glsl_toCpp() {
	std::string r = "";

	r += std::format(R"(
smq::Material* mat[{}];


void CompileShaderProgram(unsigned int& program, const char* Code, unsigned int type) {{
    program = glCreateShader(type);
    glShaderSource(program, 1, &Code, nullptr);
    glCompileShader(program);

    int ok;
    glGetShaderiv(program, GL_COMPILE_STATUS, &ok);
    if (!ok) {{
        char log[1024];
        glGetShaderInfoLog(program, 1024, nullptr, log);
        smq::Error(log);
    }}
}}


unsigned int Shader(unsigned int vs,unsigned int fs) {{
    unsigned int i_shader = glCreateProgram();

    glAttachShader(i_shader, vs);
    glAttachShader(i_shader, fs);
    glLinkProgram(i_shader);
    return i_shader;
}}

smq::Material* GetMaterial(Materials material) {{
	return mat[static_cast<unsigned int>(material)];
}}

void Iglsl() {{
)",shaders.size());


    std::vector<Shader> uniqeShaders;

    for (int i = 0; i < shaders.size(); i++) {
        std::string& vs = shaders[i].VSpath;
        bool vsE = false;
        for (int j = 0; j < uniqeShaders.size(); j++) {
            if (vs == uniqeShaders[j].name) {
                vsE = true;
            }
        }
        if (!vsE) uniqeShaders.push_back({ "GL_VERTEX_SHADER",  shaders[i].VSpath, "", });
        std::string& fs = shaders[i].FSpath;
        bool fsE = false;
        for (int j = 0; j < uniqeShaders.size(); j++) {
            if (fs == uniqeShaders[j].name) {
                fsE = true;
            }
        }
        if (!fsE) uniqeShaders.push_back({ "GL_FRAGMENT_SHADER", shaders[i].FSpath, "" });
    }

    for (int i = 0; i < uniqeShaders.size(); i++) {
        r += std::format(R"(
unsigned int {};
CompileShaderProgram({},R"(
#version {} {}

)", NoniString(uniqeShaders[i].VSpath), NoniString(uniqeShaders[i].VSpath), GetParent()->FindComponent<Control>()->i_glslVer, GetParent()->FindComponent<Control>()->i_glslMode);

        if (GetParent()->FindComponent<Control>()->i_glslMode == "es") {
            r += "precision mediump float;\n";
        }

        std::string sh = "resources/assets/" + uniqeShaders[i].VSpath;
        r += smq::LoadFile(sh);
        r += std::format(")\", {}); \n", NoniString(uniqeShaders[i].name));
    }

    for (int i = 0; i < shaders.size(); i++) {
        r += std::format(R"(
unsigned int shaders{} = Shader({}, {});

)", i, NoniString(shaders[i].VSpath), NoniString(shaders[i].FSpath));
    }

    for (int i = 0; i < uniqeShaders.size(); i++) {
        r += std::format(R"(
glDeleteShader({});
)", NoniString(uniqeShaders[i].VSpath));
    }
    r += "\n\n";

    for (int i = 0; i < shaders.size(); i++) {

        std::vector<Unitform> vsUni = GetUniforms(smq::LoadFile("resources/assets/" + shaders[i].VSpath));
        std::vector<Unitform> fsUni = GetUniforms(smq::LoadFile("resources/assets/" + shaders[i].FSpath));

        r += std::format(R"(
std::vector<unsigned int> uniforms_{};
uniforms_{}.resize({});)", shaders[i].name, shaders[i].name, vsUni.size() + fsUni.size());

        r += std::format(R"(
std::vector<smq::GlAtribute> atributes_{};
atributes_{}.resize({});)", shaders[i].name, shaders[i].name, vsUni.size() + fsUni.size());

        for (int j = 0; j < vsUni.size(); j++) {
            r += std::format(R"(
uniforms_{}[{}] = glGetUniformLocation(shaders{}, "{}");)", shaders[i].name,j,i, vsUni[j].name);

            r += std::format(R"(
atributes_{}[{}].type = smq::GlUniform_{};)", shaders[i].name, j, UniformToNString(vsUni[j].type));

            r += std::format(R"(
atributes_{}[{}].)", shaders[i].name, j);

            std::string val = UniformToNString(vsUni[j].type);

            if (val == "Float") {
                r += "f = 0.0f;\n";
            }

            if (val == "Vec2") {
                r += "f2 = {0.0f,0.0f};\n";
            }

            if (val == "Vec3") {
                r += "f3 = {0.0f,0.0f,0,0f};\n";
            }

            if (val == "Vec4") {
                r += "f4 = {0.0f,0.0f,0.0f,0.0f};\n";
            }

            if (val == "Int") {
                r += "i = 0;\n";
            }

            if (val == "Ivec2") {
                r += "i2 = {0,0};\n";
            }

            if (val == "Ivec3") {
                r += "i3 = {0,0,0};\n";
            }

            if (val == "Ivec4") {
                r += "i4 = {0,0,0,0};\n";
            }

            if (val == "Mat3") {
                r += "m3 = {0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f};\n";
            }

            if (val == "Mat4") {
                r += "m4 = {0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f};\n";
            }
        }
        for (int j = 0; j < fsUni.size(); j++) {
            r += std::format(R"(
uniforms_{}[{}] = glGetUniformLocation(shaders{}, "{}");)", shaders[i].name, vsUni.size() + j, i, fsUni[j].name);

            r += std::format(R"(
atributes_{}[{}].type = smq::GlUniform_{};)", shaders[i].name, vsUni.size() + j, UniformToNString(fsUni[j].type));

            r += std::format(R"(
atributes_{}[{}].)", shaders[i].name, vsUni.size() + j);


            std::string val = UniformToNString(fsUni[j].type);

            if (val == "Float") {
                r += "f = 0.0f;\n";
            }

            if (val == "Vec2") {
                r += "f2 = {0.0f,0.0f};\n";
            }

            if (val == "Vec3") {
                r += "f3 = {0.0f,0.0f,0,0f};\n";
            }

            if (val == "Vec4") {
                r += "f4 = {0.0f,0.0f,0.0f,0.0f};\n";
            }

            if (val == "Int") {
                r += "i = 0;\n";
            }

            if (val == "Ivec2") {
                r += "i2 = {0,0};\n";
            }

            if (val == "Ivec3") {
                r += "i3 = {0,0,0};\n";
            }

            if (val == "Ivec4") {
                r += "i4 = {0,0,0,0};\n";
            }

            if (val == "Mat3") {
                r += "m3 = {0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f};\n";
            }

            if (val == "Mat4") {
                r += "m4 = {0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,0.0f};\n";
            }
        }



        r += std::format(R"(
mat[{}] = new smq::Material(shaders{}, uniforms_{}, atributes_{});
)", i,i, shaders[i].name, shaders[i].name);
        for (int j = 0; j < shaders[i].textures.size(); j++) {
            r += std::format("mat[{}]->AddTexture(img[{}]);", i, shaders[i].textures[j]);
        }
    }

	r += std::format(R"(
}}

void Qglsl() {{
	for (int i = 0; i < {}; i++) {{
		delete mat[i];
	}}
}}

)", shaders.size());

	return r;	
}
