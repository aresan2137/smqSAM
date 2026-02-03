#include "smf.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



std::string Smf::smf_toCpp() {
	std::string r = "";

	r += std::format("smq::Mesh* smf[{}];", models.size());

    r += R"(
smq::Mesh* GetMesh(Meshes mesh) {
    return smf[static_cast<unsigned int>(mesh)];
}

void Ismf() {
)";

    for (int i = 0; i < models.size(); i++) {
        MeshData mesh;

        if (LoadModel("resources/assets/" + models[i].path, mesh)) {
            r += std::format("smf[{}] = new smq::Mesh({{", i);
            // verticies
            for (int j = 0; j < mesh.vertices.size(); j++) {
                r += std::to_string(mesh.vertices[j].px) + "f,";
                r += std::to_string(mesh.vertices[j].py) + "f,";
                r += std::to_string(mesh.vertices[j].pz) + "f,";
                r += std::to_string(mesh.vertices[j].u) + "f,";
                r += std::to_string(mesh.vertices[j].v) + "f,";
                r += std::to_string(mesh.vertices[j].nx) + "f,";
                r += std::to_string(mesh.vertices[j].ny) + "f,";
                r += std::to_string(mesh.vertices[j].nz) + "f,\n";
            }
            // indicies
            r += "\n},{\n";
            for (int j = 0; j < mesh.indices.size(); j++) {
                r += std::to_string(mesh.indices[j]) + ",";
                if (j % 3 == 0) r += "\n";
            }
            // layout
            r += "}, {3,2,3});\n";
        } else {
            smq::Error("Failed to load model: " + models[i].name);
        }
    }

    r += std::format(R"(
}}

void Qsmf() {{
    for (int i = 0; i < {}; i++) {{
        delete smf[i];
    }}
}}

)", models.size());

	return r;	
}

bool LoadModel(const std::string& path,MeshData& outMesh) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_OptimizeMeshes
    );

    if (!scene || !scene->HasMeshes()) {
        smq::Error(("Assimp error: " + (std::string)importer.GetErrorString()));
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    outMesh.vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v{};

        v.px = mesh->mVertices[i].x;
        v.py = mesh->mVertices[i].y;
        v.pz = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            v.nx = mesh->mNormals[i].x;
            v.ny = mesh->mNormals[i].y;
            v.nz = mesh->mNormals[i].z;
        }

        if (mesh->HasTextureCoords(0)) {
            v.u = mesh->mTextureCoords[0][i].x;
            v.v = mesh->mTextureCoords[0][i].y;
        } else {
            v.u = 0.0f;
            v.v = 0.0f;
        }

        outMesh.vertices.push_back(v);
    }

    outMesh.indices.reserve(mesh->mNumFaces * 3);

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        if (face.mNumIndices != 3) {
            continue;
        }

        outMesh.indices.push_back(face.mIndices[0]);
        outMesh.indices.push_back(face.mIndices[1]);
        outMesh.indices.push_back(face.mIndices[2]);
    }

    return true;
}