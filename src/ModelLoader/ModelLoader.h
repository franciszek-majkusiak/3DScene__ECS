#pragma once

#include "Components/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "LibFiles/stb_image.h"
#include "Components/Mesh.h"


unsigned int TextureFromFile(const char* path, const std::string& directory = "", bool gamma = false);
Model loadModelFromObj(std::string path);
Model buildTerrainMesh(float width, float length, int resolution, std::string diffuseTexturePath = "", std::string normalsTexturePath = "");

class ModelLoader
{
public:
	ModelLoader(std::string path);
	Model ConstructModel();
private:
	// model data
	std::string directory;
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
