#pragma once

#include <fstream> 

#include "Log.h"

namespace smq {
    inline std::string LoadFile(std::string Filename) {
        std::ifstream file(Filename, std::ios::binary);

        if (!file) Error("LoadFile() file loading failed. File Not Found: " + Filename);

        std::string content(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        if (content.size() >= 3 &&
            (unsigned char)content[0] == 0xEF &&
            (unsigned char)content[1] == 0xBB &&
            (unsigned char)content[2] == 0xBF) {
            content.erase(0, 3);
        }

        return content;
    }

    inline void SaveFile(const std::string Filename, const std::string& Data) {
        std::ofstream file(Filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + Filename);
        }

        unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
        file.write(reinterpret_cast<const char*>(bom), 3);

        file.write(Data.c_str(), Data.size());

        file.close();
    }
}