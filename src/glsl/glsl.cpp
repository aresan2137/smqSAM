#include "glsl.h"
#include "../img/img.h"

Glsl::Glsl() {
    std::ifstream file("resources/glsl.json");
    json data;
    file >> data;
    shaders.reserve(data["Shaders"].size());
    for (int i = 0; i < data["Shaders"].size(); i++) {
        shaders.emplace_back(data["Shaders"][i]["Name"], data["Shaders"][i]["VSFilePath"], data["Shaders"][i]["FSFilePath"]);
        if (data["Shaders"][i].contains("Textures")) {
            shaders[i].textures.reserve(data["Shaders"][i]["Textures"].size());
            for (int j = 0; j < data["Shaders"][i]["Textures"].size(); j++) {
                shaders[i].textures.push_back(data["Shaders"][i]["Textures"][j]["ID"]);
            }
        }		
    }
}

Glsl::~Glsl() {
    Save();
}

void Glsl::Save() {
    json data;

    for (int i = 0; i < shaders.size(); i++) {
        json imageJson;
        imageJson["Name"] = shaders[i].name;
        imageJson["VSFilePath"] = shaders[i].VSpath;
        imageJson["FSFilePath"] = shaders[i].FSpath;

        for (int j = 0; j < shaders[i].textures.size(); j++) {
            json texu;
            texu["ID"] = shaders[i].textures[j];
            imageJson["Textures"].push_back(texu);
        }

        data["Shaders"].push_back(imageJson);
    }

    std::ofstream file("resources/glsl.json");
    if (file.is_open()) {
        file << data.dump(2);
    }
}

void Glsl::Start() {
	
}

void Glsl::Update(float Delta) {
    ImGui::Begin("Materials");

    if (ImGui::Button("Add")) i_adding = true;
    ImGui::SameLine();
    if (ImGui::Button("Edit")) {
        if (chosenShader < 0 || chosenShader >= shaders.size()) i_edit = false;
        else {
            i_edit = true;
            strncpy(i_Name, shaders[chosenShader].name.c_str(), sizeof(i_Name) - 1);
            strncpy(i_VSFileName, shaders[chosenShader].VSpath.c_str(), sizeof(i_VSFileName) - 1);
            strncpy(i_FSFileName, shaders[chosenShader].FSpath.c_str(), sizeof(i_FSFileName) - 1);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        if (chosenShader < 0 || chosenShader >= shaders.size()) {
        } else {
            shaders.erase(shaders.begin() + chosenShader);
        }
    }

    ImGui::Separator();

    if (ImGui::BeginListBox("##listbox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (int n = 0; n < shaders.size(); n++) {
            const bool is_selected = (chosenShader == n);

            std::string label = shaders[n].name + "##item_" + std::to_string(n);

            if (ImGui::Selectable(label.c_str(), is_selected)) {
                chosenShader = n;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

	ImGui::Separator();
    if (ImGui::Button("Texture Edit")) {
        if (chosenShader < 0 || chosenShader >= shaders.size()) i_texturing = false;
        else {
            i_texturing = true;
        }
    } 

    ImGui::End();

    if (i_texturing) {
        ImGui::Begin("Link Textures");

		Img* imgu = GetParent()->FindComponent<Img>();

        if (ImGui::BeginListBox("##images", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < imgu->images.size(); n++) {
                const bool is_selected = (chosenTexture == n);

                std::string label = imgu->images[n].name + "##item_" + std::to_string(n);

                if (ImGui::Selectable(label.c_str(), is_selected)) {
                    chosenTexture = n;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndListBox();
        }
        if (ImGui::BeginListBox("##added", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < shaders[chosenShader].textures.size(); n++) {
                const bool is_selected = (chosenTexture2 == n);

                std::string label = imgu->images[shaders[chosenShader].textures[n]].name + "##item_" + std::to_string(n);

                if (ImGui::Selectable(label.c_str(), is_selected)) {
                    chosenTexture2 = n;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndListBox();
        }

        if (ImGui::Button("Add")) {
			shaders[chosenShader].textures.push_back(chosenTexture);
        }

        if (ImGui::Button("Remove")) {
            shaders[chosenShader].textures.erase(shaders[chosenShader].textures.begin() + chosenTexture2);
        }

        if (ImGui::Button("back")) {
            i_texturing = false;
        }

        ImGui::End();
    }

    if (i_edit) {
        ImGui::Begin("Edit Materials");

        ImGui::InputText("Name", i_Name, sizeof(i_Name));
        ImGui::InputText("Vertex Shader FilePath", i_VSFileName, sizeof(i_VSFileName));
        ImGui::InputText("Fragment Shader FilePath", i_FSFileName, sizeof(i_FSFileName));

        if (ImGui::Button("OK")) {
            std::string name(i_Name);
            std::string Vpath(i_VSFileName);
            std::string Fpath(i_FSFileName);

            if (Vpath[0] != 0 && Fpath[0] != 0 && name[0] != 0) {
                shaders[chosenShader].name = name;
                shaders[chosenShader].VSpath = Vpath;
                shaders[chosenShader].FSpath = Fpath;
            }

            i_VSFileName[0] = 0;
            i_FSFileName[0] = 0;
            i_Name[0] = 0;
            i_edit = false;
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            i_VSFileName[0] = 0;
            i_FSFileName[0] = 0;
            i_Name[0] = 0;
            i_edit = false;
        }

        ImGui::End();
    }

    if (i_adding) {
        ImGui::Begin("Add Materials");

        ImGui::InputText("Name", i_Name, sizeof(i_Name));
        ImGui::InputText("Vertex Shader FilePath", i_VSFileName, sizeof(i_VSFileName));
        ImGui::InputText("Fragment Shader FilePath", i_FSFileName, sizeof(i_FSFileName));

        if (ImGui::Button("Add")) {
            std::string name(i_Name);
            std::string Vpath(i_VSFileName);
            std::string Fpath(i_FSFileName);

            if (Vpath[0] != 0 && Fpath[0] != 0 && name[0] != 0) {
                Shader shd = Shader(name,Vpath,Fpath);
                shaders.push_back(shd);
            }

            i_VSFileName[0] = 0;
            i_FSFileName[0] = 0;
            i_Name[0] = 0;
            i_adding = false;
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            i_VSFileName[0] = 0;
            i_FSFileName[0] = 0;
            i_Name[0] = 0;
            i_adding = false;
        }

        ImGui::End();
    }
}

std::string Glsl::UniformToNString(std::string type) {
    if (type == "float")  return "Float";
    if (type == "vec2")   return "Vec2";
    if (type == "vec3")   return "Vec3";
    if (type == "vec4")   return "Vec4";

    if (type == "int")    return "Int";
    if (type == "ivec2")  return "IVec2";
    if (type == "ivec3")  return "IVec3";
    if (type == "ivec4")  return "IVec4";

    if (type == "mat3")   return "Mat3";
    if (type == "mat4")   return "Mat4";

    if (type == "sampler2D")
        return "Int";

    smq::Error("Unknown uniform type: " + type);
    return "Unknown";
}


std::vector<Unitform> Glsl::GetUniforms(std::string src) {
    std::vector<Unitform> out;

    int i = 0;
    int n = src.size();

    auto is_ws = [](char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
        };

    auto is_ident = [](char c) {
        return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_';
        };

    while (i < n) {

        if (i + 7 <= n &&
            src.compare(i, 7, "uniform") == 0 &&
            (i == 0 || !is_ident(src[i - 1])) &&
            (i + 7 == n || is_ws(src[i + 7]))) {

            i += 7;

            while (i < n && is_ws(src[i])) i++;
            int type_start = i;
            while (i < n && is_ident(src[i])) i++;
            std::string type = src.substr(type_start, i - type_start);

            while (i < n && is_ws(src[i])) i++;
            int name_start = i;
            while (i < n && is_ident(src[i])) i++;
            std::string name = src.substr(name_start, i - name_start);

            int br = name.find('[');
            if (br != std::string::npos)
                name = name.substr(0, br);

            if (!type.empty() && !name.empty()) {
                out.push_back({ name, type });
            }
        }

        i++;
    }

    return out;
}