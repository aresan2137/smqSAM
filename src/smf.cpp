#include "include.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    float px, py, pz;
    float nx, ny, nz;
    float u, v;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

bool LoadModel(
    const std::string& path,
    MeshData& outMesh
) {
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
        std::cerr << "Assimp error: " << importer.GetErrorString() << "\n";
        return false;
    }

    // Na razie: bierzemy tylko pierwszy mesh
    aiMesh* mesh = scene->mMeshes[0];

    outMesh.vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v{};

        // Position
        v.px = mesh->mVertices[i].x;
        v.py = mesh->mVertices[i].y;
        v.pz = mesh->mVertices[i].z;

        // Normal
        if (mesh->HasNormals()) {
            v.nx = mesh->mNormals[i].x;
            v.ny = mesh->mNormals[i].y;
            v.nz = mesh->mNormals[i].z;
        }

        // UV (kanał 0)
        if (mesh->HasTextureCoords(0)) {
            v.u = mesh->mTextureCoords[0][i].x;
            v.v = mesh->mTextureCoords[0][i].y;
        } else {
            v.u = 0.0f;
            v.v = 0.0f;
        }

        outMesh.vertices.push_back(v);
    }

    // Indices
    outMesh.indices.reserve(mesh->mNumFaces * 3);

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];

        // Po triangulacji zawsze 3
        if (face.mNumIndices != 3) {
            continue;
        }

        outMesh.indices.push_back(face.mIndices[0]);
        outMesh.indices.push_back(face.mIndices[1]);
        outMesh.indices.push_back(face.mIndices[2]);
    }

    return true;
}

std::string smfComp(Config config, std::vector<SmfConf> smf) {
	std::string h = "";

    h += "meshes.reserve(";
    h += std::to_string(smf.size());
    h += ");\n";

	for (int i = 0; i < smf.size(); i++) {
        MeshData mesh; // std::vector<smq::Mesh> meshes;    

        if (LoadModel("resources/assets/" + smf[i].fl, mesh)) {
            h += "meshes.push_back(smq::Mesh({";
            for (int j = 0; j < mesh.vertices.size(); j++) {
                h += std::to_string(mesh.vertices[j].px) + "f,";
                h += std::to_string(mesh.vertices[j].py) + "f,";
                h += std::to_string(mesh.vertices[j].pz) + "f,";
                h += std::to_string(mesh.vertices[j].u) + "f,";
                h += std::to_string(mesh.vertices[j].v) + "f,"; 
                h += std::to_string(mesh.vertices[j].nx) + "f,";
                h += std::to_string(mesh.vertices[j].ny) + "f,";
                h += std::to_string(mesh.vertices[j].nz) + "f,\n";
            }

            h += "\n},{\n";
            for (int j = 0; j < mesh.indices.size(); j++) {
                h += std::to_string(mesh.indices[j]) + ",";
                if (j % 3 == 0) h += "\n";
            }

            h += "}, {3,2,3}));\n";
        } else {
            smq::Error("Failed to load model: " + smf[i].name);
        }

	}

	return h;
}