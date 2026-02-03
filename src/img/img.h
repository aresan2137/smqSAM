#pragma once

#include "../Include.h"

struct Image {
	std::string name;
	std::string path;

	Image(std::string n, std::string p) : name(n), path(p) {}
};

class Img : public smq::Component {
public:
	Img();
	~Img();

	void Start() override;
	void Update(float Delta) override;

	void Save();

	std::string img_toCpp();
	std::string img_toH();

	std::vector<Image> images;

private:
	char i_FileName[256] = "";
	char i_Name[256] = "";

	bool i_adding = false;
	bool i_edit = false;

	
	int chosenImage = -1;
};