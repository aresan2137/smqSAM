#include "smf.h"

Smf::Smf() {
    std::ifstream file("resources/smf.json");
    json data;
    file >> data;
    models.reserve(data["Models"].size());
    for (int i = 0; i < data["Models"].size(); i++) {
        models.emplace_back(data["Models"][i]["Name"], data["Models"][i]["FilePath"]);
    }
}

Smf::~Smf() {
    Save();
}

void Smf::Save() {
    json data;

    for (int i = 0; i < models.size(); i++) {
        json modelJson;
        modelJson["Name"] = models[i].name;
        modelJson["FilePath"] = models[i].path;

        data["Models"].push_back(modelJson);
    }

    std::ofstream file("resources/smf.json");
    if (file.is_open()) {
        file << data.dump(2);
    }
}

void Smf::Start() {
	
}

void Smf::Update(float Delta) {
    ImGui::Begin("Models");

    if (ImGui::Button("Add")) i_adding = true;
    ImGui::SameLine();
    if (ImGui::Button("Edit")) {
        if (chosenModel < 0 || chosenModel >= models.size()) i_edit = false;
        else {
            i_edit = true;
            strncpy(i_Name, models[chosenModel].name.c_str(), sizeof(i_Name) - 1);
            strncpy(i_FileName, models[chosenModel].path.c_str(), sizeof(i_FileName) - 1);
        }

    }
    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        if (chosenModel < 0 || chosenModel >= models.size()) {
        } else {
            models.erase(models.begin() + chosenModel);
        }
    }

    ImGui::Separator();

    if (ImGui::BeginListBox("##listbox", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (int n = 0; n < models.size(); n++) {
            const bool is_selected = (chosenModel == n);

            std::string label = models[n].name + "##item_" + std::to_string(n);

            if (ImGui::Selectable(label.c_str(), is_selected)) {
                chosenModel = n;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }

    ImGui::Separator();

    if (ImGui::Button("Previev")) {
        if (chosenModel < 0 || chosenModel >= models.size()) i_edit = false;
        else {
            MeshData datae;
            LoadModel("resources/assets/" + models[chosenModel].path, datae);
            std::vector<float> rawVertices;
            size_t floatCount = datae.vertices.size() * 8;
            rawVertices.resize(floatCount);
            std::memcpy(rawVertices.data(), datae.vertices.data(), datae.vertices.size() * sizeof(Vertex));

            smq::Mesh* prewMesh = new smq::Mesh(rawVertices, datae.indices, std::vector<unsigned int>{ 3, 2, 3 });

            GetParent()->GetAllChildren()->at(0)->GetModelMaterial()->SetModel(prewMesh);
        }
    }

    ImGui::End();

    if (i_edit) {


        ImGui::Begin("Edit Model");

        ImGui::InputText("Name", i_Name, sizeof(i_Name));
        ImGui::InputText("FilePath", i_FileName, sizeof(i_FileName));

        if (ImGui::Button("OK")) {
            std::string name(i_Name);
            std::string path(i_FileName);

            if (path[0] != 0 && name[0] != 0) {
                models[chosenModel].name = name;
                models[chosenModel].path = path;
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
        ImGui::Begin("Add Model");

        ImGui::InputText("Name", i_Name, sizeof(i_Name));
        ImGui::InputText("FilePath", i_FileName, sizeof(i_FileName));

        if (ImGui::Button("Add")) {
            std::string name(i_Name);
            std::string path(i_FileName);

            if (path[0] != 0 && name[0] != 0) {
                Model mdl = Model(name,path);
                models.push_back(mdl);
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