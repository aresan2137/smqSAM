#pragma once

#include "../Include.h"

class Control : public smq::Component {
public:
	Control();
	~Control();

	void Start() override;
	void Update(float Delta) override;

	std::string i_glslVer;
	std::string i_glslMode;

private:
	char i_FileName[256] = "";

	void Compile();
};