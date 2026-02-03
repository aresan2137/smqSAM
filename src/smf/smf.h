#pragma once

#include "../Include.h"

struct Model {
	std::string name;
	std::string path;

	Model(std::string n, std::string p) : name(n), path(p) {}
};

class Smf : public smq::Component {
public:
	Smf();
	~Smf();

	void Start() override;
	void Update(float Delta) override;

	void Save();

	std::string smf_toCpp();
	std::string smf_toH();

private:
	char i_FileName[256] = "";
	char i_Name[256] = "";

	bool i_adding = false;
	bool i_edit = false;

	std::vector<Model> models;
	int chosenModel = -1;
};