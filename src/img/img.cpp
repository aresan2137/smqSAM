#include "img.h"

Img::Img() {
    std::ifstream file("resources/img.json");
    json data;
    file >> data;
    images.reserve(data["Images"].size());
    for (int i = 0; i < data["Images"].size(); i++) {
        images.emplace_back(data["Images"][i]["Name"], data["Images"][i]["FilePath"]);
    }
}

Img::~Img() {
    Save();
}

void Img::Save() {
    json data;

    for (int i = 0; i < images.size(); i++) {
        json imageJson;
        imageJson["Name"] = images[i].name;
        imageJson["FilePath"] = images[i].path;

        data["Images"].push_back(imageJson);
    }

    std::ofstream file("resources/img.json");
    if (file.is_open()) {
        file << data.dump(2);
    }
}

void Img::Start() {
	
}

void Img::Update(float Delta) {
    ImGui::Begin("Image");

    if (ImGui::Button("Add")) i_adding = true;
    ImGui::SameLine();
    if (ImGui::Button("Edit")) {
        if (chosenImage < 0 || chosenImage >= images.size()) i_edit = false;
        else {
            i_edit = true;
            strncpy(i_Name, images[chosenImage].name.c_str(), sizeof(i_Name) - 1);
            strncpy(i_FileName, images[chosenImage].path.c_str(), sizeof(i_FileName) - 1);
        }

    }
    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        if (chosenImage < 0 || chosenImage >= images.size()) {
        } else {
            images.erase(images.begin() + chosenImage);
        }
    }

    ImGui::Separator();

    if (ImGui::BeginListBox("##listbox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (int n = 0; n < images.size(); n++) {
            const bool is_selected = (chosenImage == n);

            std::string label = images[n].name + "##item_" + std::to_string(n);

            if (ImGui::Selectable(label.c_str(), is_selected)) {
                chosenImage = n;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    ImGui::End();

    if (i_edit) {


        ImGui::Begin("Edit Image");

        ImGui::InputText("Name", i_Name, sizeof(i_Name));
        ImGui::InputText("FilePath", i_FileName, sizeof(i_FileName));

        if (ImGui::Button("OK")) {
            std::string name(i_Name);
            std::string path(i_FileName);

            if (path[0] != 0 && name[0] != 0) {
                images[chosenImage].name = name;
                images[chosenImage].path = path;
            }

            i_FileName[0] = 0;
            i_Name[0] = 0;
            i_edit = false;
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            i_FileName[0] = 0;
            i_Name[0] = 0;
            i_edit = false;
        }

        ImGui::End();
    }

    if (i_adding) {
        ImGui::Begin("Add Image");

        ImGui::InputText("Name", i_Name, sizeof(i_Name));
        ImGui::InputText("FilePath", i_FileName, sizeof(i_FileName));

        if (ImGui::Button("Add")) {
            std::string name(i_Name);
            std::string path(i_FileName);

            if (path[0] != 0 && name[0] != 0) {
                Image mdl = Image(name,path);
                images.push_back(mdl);
            }

            i_FileName[0] = 0;
            i_Name[0] = 0;
            i_adding = false;
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            i_Name[0] = 0;
            i_FileName[0] = 0;
            i_adding = false;
        }

        ImGui::End();
    }
}