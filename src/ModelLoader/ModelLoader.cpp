#include "ModelLoader.h"



Model loadModelFromObj(std::string path)
{
	ModelLoader modelLoader(path);
	return modelLoader.ConstructModel();
}


ModelLoader::ModelLoader(std::string path)
{
	loadModel(path);
}

Model ModelLoader::ConstructModel()
{
	Model model;
	model.meshes = meshes;
	return model;
}




void ModelLoader::loadModel(std::string const& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		fprintf(stderr, "ERROR::ASSIMP::%s\n", importer.GetErrorString());
		exit(EXIT_FAILURE);
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}


void ModelLoader::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node’s meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texVec;
			texVec.x = mesh->mTextureCoords[0][i].x;
			texVec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texVec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];
		glm::vec3 Edge1 = v1.Position - v0.Position;
		glm::vec3 Edge2 = v2.Position - v0.Position;

		float DeltaU1 = v1.TexCoords.x - v0.TexCoords.x;
		float DeltaV1 = v1.TexCoords.y - v0.TexCoords.y;
		float DeltaU2 = v2.TexCoords.x - v0.TexCoords.x;
		float DeltaV2 = v2.TexCoords.y - v0.TexCoords.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		glm::vec3 Tangent, Bitangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Bitangent.x = f * (-DeltaU2 * Edge1.x + DeltaU1 * Edge2.x);
		Bitangent.y = f * (-DeltaU2 * Edge1.y + DeltaU1 * Edge2.y);
		Bitangent.z = f * (-DeltaU2 * Edge1.z + DeltaU1 * Edge2.z);

		v0.Tangent += Tangent;
		v1.Tangent += Tangent;
		v2.Tangent += Tangent;
	}

	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
	}
	Material meshMaterial;
	if (mesh->mMaterialIndex >= 0)
	{

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString str;
		aiColor3D color;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = "texture_diffuse";
			texture.path = str.C_Str();
			meshMaterial.texture_diffuse = texture;
			meshMaterial.isUsingDiffuseTexture = true;
		}
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		meshMaterial.ambient = 0.1f * glm::vec3(color.r, color.g, color.b);
		if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			material->GetTexture(aiTextureType_SPECULAR, 0, &str);
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = "texture_specular";
			texture.path = str.C_Str();
			meshMaterial.texture_specular = texture;
			meshMaterial.isUsingSpecularTexture = true;
		}
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		meshMaterial.diffuse = glm::vec3(color.r, color.g, color.b);
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			material->GetTexture(aiTextureType_NORMALS, 0, &str);
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = "texture_normals";
			texture.path = str.C_Str();
			meshMaterial.texture_normals = texture;
			meshMaterial.isUsingNormalsTexture = true;
		}
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		meshMaterial.specular = glm::vec3(color.r, color.g, color.b);
		float shininess;
		aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
		meshMaterial.shininess = shininess;
	}

	Mesh newMesh;

	newMesh.indicesSize = indices.size();
	newMesh.verticesSize = vertices.size();
	newMesh.vertices = std::make_shared<Vertex[]>(vertices.size());
	newMesh.indices = std::make_shared<unsigned int[]>(indices.size());
	std::copy(vertices.begin(), vertices.end(), newMesh.vertices.get());
	std::copy(indices.begin(), indices.end(), newMesh.indices.get());


	glGenVertexArrays(1, &newMesh.VAO);
	glGenBuffers(1, &newMesh.VBO);
	glGenBuffers(1, &newMesh.EBO);
	glBindVertexArray(newMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, newMesh.verticesSize * sizeof(Vertex), newMesh.vertices.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.indicesSize * sizeof(unsigned int), newMesh.indices.get(), GL_STATIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent (normal mapping)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glBindVertexArray(0);


	newMesh.material = meshMaterial;
	return newMesh;
}

std::vector<Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(),
				str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{ // if texture hasn’t been loaded already, load it
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	if(directory != "")
		filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else
	{
		printf("Texture failed to load at path: %s\n",path);
		stbi_image_free(data);
	}

	return textureID;
}


Texture isTextureLoaded(std::vector<Texture> textures, std::string texturePath)
{
	auto is_same = [texturePath](Texture t) {return t.path == texturePath; };
	auto it = find_if(textures.begin(), textures.end(), is_same);
	if (it != textures.end())
	{
		return *it;
	}
	Texture empty;
	empty.ID = -1;
	return empty;
}




Model buildTerrainMesh(float width, float length, int resolution, std::string diffuseTexturePath, std::string normalsTexturePath)
{
	float stepX = width / (float)(resolution - 1);
	float stepZ = length / (float)(resolution - 1);
	float x = 0, z = 0;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (int i = 0; i < resolution; i++)
	{
		for (int j = 0; j < resolution; j++)
		{
			x = i * stepX; z = j * stepZ;
			glm::vec3 position = glm::vec3(x, 0, z);
			glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec2 texCoords = glm::vec2(x / width, z / length);
			Vertex vertex;
			vertex.Position = position;
			vertex.Normal = normal;
			vertex.TexCoords = texCoords;
			vertices.push_back(vertex);
			if (i < resolution - 1 && j < resolution - 1)
			{
				indices.push_back(j * resolution + i);
				indices.push_back(j * resolution + i + 1);
				indices.push_back((j + 1) * resolution + i + 1);

				indices.push_back(j * resolution + i);
				indices.push_back((j + 1) * resolution + i + 1);
				indices.push_back((j + 1) * resolution + i);
			}

		}
	}


	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];
		glm::vec3 Edge1 = v1.Position - v0.Position;
		glm::vec3 Edge2 = v2.Position - v0.Position;

		float DeltaU1 = v1.TexCoords.x - v0.TexCoords.x;
		float DeltaV1 = v1.TexCoords.y - v0.TexCoords.y;
		float DeltaU2 = v2.TexCoords.x - v0.TexCoords.x;
		float DeltaV2 = v2.TexCoords.y - v0.TexCoords.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		glm::vec3 Tangent, Bitangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Bitangent.x = f * (-DeltaU2 * Edge1.x + DeltaU1 * Edge2.x);
		Bitangent.y = f * (-DeltaU2 * Edge1.y + DeltaU1 * Edge2.y);
		Bitangent.z = f * (-DeltaU2 * Edge1.z + DeltaU1 * Edge2.z);

		v0.Tangent += Tangent;
		v1.Tangent += Tangent;
		v2.Tangent += Tangent;
	}

	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
	}
	Material material;
	if (diffuseTexturePath != "")
	{
		Texture texture;
		texture.ID = TextureFromFile(diffuseTexturePath.c_str());
		texture.type = "texture_diffuse";
		texture.path = diffuseTexturePath;
		material.texture_diffuse = texture;
		material.isUsingDiffuseTexture = true;
	}
	if (normalsTexturePath != "")
	{
		Texture texture;
		texture.ID = TextureFromFile(normalsTexturePath.c_str());
		texture.type = "texture_normals";
		texture.path = normalsTexturePath;
		material.texture_normals = texture;
		material.isUsingNormalsTexture = true;
	}
	material.diffuse = glm::vec3(0.0f, 0.4f, 0.0f);
	material.specular = 0.5f * material.diffuse;
	material.ambient = 0.1f * material.diffuse;
	
	Mesh terrainMesh;

	terrainMesh.vertices = std::make_shared<Vertex[]>(vertices.size());
	terrainMesh.indices = std::make_shared<unsigned int[]>(indices.size());
	std::copy(vertices.begin(), vertices.end(), terrainMesh.vertices.get());
	std::copy(indices.begin(), indices.end(), terrainMesh.indices.get());
	terrainMesh.indicesSize = indices.size();
	terrainMesh.verticesSize = vertices.size();


	terrainMesh.material = material;

	glGenVertexArrays(1, &terrainMesh.VAO);
	glGenBuffers(1, &terrainMesh.VBO);
	glGenBuffers(1, &terrainMesh.EBO);
	glBindVertexArray(terrainMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainMesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, terrainMesh.verticesSize * sizeof(Vertex), terrainMesh.vertices.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainMesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainMesh.indicesSize * sizeof(unsigned int), terrainMesh.indices.get(), GL_STATIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent (normal mapping)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glBindVertexArray(0);

	Model terrain;
	terrain.meshes.push_back(terrainMesh);
	return terrain;

}
