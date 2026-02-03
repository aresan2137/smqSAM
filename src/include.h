#pragma once

#include <string>
#include <vector>
#include <format>


#include "json.hpp"
using json = nlohmann::json;

#include <Smq.h>



inline std::string NoniString(const std::string& input) {

    std::string result = "";

    if (std::isdigit(static_cast<unsigned char>(input[0]))) {
        result += '_';
    }

    for (unsigned char c : input) {
        if (std::isalnum(c) || c == '_') {
            result += c;
        } else {
            result += '_';
        }
    }
    std::string finalResult;
    bool lastWasUnderscore = false;
    for (char c : result) {
        if (c == '_') {
            if (!lastWasUnderscore) {
                finalResult += c;
                lastWasUnderscore = true;
            }
        } else {
            finalResult += c;
            lastWasUnderscore = false;
        }
    }

    if (finalResult == "int" || finalResult == "void" || finalResult == "float") {
        finalResult = "_" + finalResult;
    }

    return finalResult;
}

struct Vertex {
    float px, py, pz;
    float u, v;
    float nx, ny, nz;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};


bool LoadModel(const std::string& path, MeshData& outMesh);