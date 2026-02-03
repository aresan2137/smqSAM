#pragma once

#include "../Include.h"

struct Shader {
	std::string name;
	std::string VSpath;
	std::string FSpath;

	std::vector<int> textures;

	Shader(std::string n, std::string v, std::string f) : name(n), VSpath(v), FSpath(f) {}
};

struct Unitform {
	std::string name;
	std::string type;
};

class Glsl : public smq::Component {
public:
	Glsl();
	~Glsl();

	void Start() override;
	void Update(float Delta) override;

	void Save();

	std::string glsl_toCpp();
	std::string glsl_toH();

private:
	char i_FSFileName[256] = "";
	char i_VSFileName[256] = "";
	char i_Name[256] = "";

	bool i_adding = false;
	bool i_edit = false;
	bool i_texturing = false;

	std::vector<Shader> shaders;
	int chosenShader = -1;

	int chosenTexture = -1;
	int chosenTexture2 = -1;

	std::string UniformToNString(std::string type);
	std::vector<Unitform> GetUniforms(std::string src);
};