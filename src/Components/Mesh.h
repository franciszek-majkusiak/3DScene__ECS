#pragma once
#include <glm/glm.hpp>
#include <string>
#include <glad/glad.h>
#include <vector>
#include <memory>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

struct Texture
{
	int ID = -1;
	std::string type = "";
	std::string path = "";
};

struct Material
{
	glm::vec3 ambient = glm::vec3(0.01f, 0.01f, 0.01f);
	glm::vec3 diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 emmision = glm::vec3(0.0f, 0.0f, 0.0f);
	bool isUsingDiffuseTexture = false;
	Texture texture_diffuse;
	bool isUsingSpecularTexture = false;
	Texture texture_specular;
	bool isUsingNormalsTexture = false;
	Texture texture_normals;
	float shininess = 2;
};
struct Mesh
{
	Material material;
	GLuint VAO = -1;
    GLuint VBO = -1;
    GLuint EBO = -1;
	std::shared_ptr<Vertex[]> vertices = nullptr;
	std::shared_ptr<unsigned int[]> indices = nullptr;
	size_t indicesSize = 0;
	size_t verticesSize = 0;
};

struct Model
{
	std::vector<Mesh> meshes;
	bool isPrefab = false;
};



Model buildCubeModel(glm::vec3 diffuse = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 emmision = glm::vec3(0.0f, 0.0f, 0.0f));



glm::vec3 pointOnCubeToPointOnSphere(glm::vec3 p);

Mesh ConstructSphereFaceMesh(float radius, int resolution, glm::vec3 localUp, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emmision);

Model buildSphereModel(float radius, int resolution, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emmision);


