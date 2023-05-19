#include "Mesh.h"
#include <glm/gtc/constants.hpp>





Model buildCubeModel(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emmision)
{
	float verticesArr[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int indicesArr[] = {
		0, 1, 2,
		3, 4, 5,

		6, 7, 8,
		9, 10, 11,

		12, 13, 14,
		15, 16, 17,

		18, 19, 20,
		21, 22, 23,

		24, 25, 26,
		27, 28, 29,

		30, 31, 32,
		33, 34, 35
	};

	Mesh cubeMesh;
	auto indices = std::vector<unsigned int>(indicesArr, indicesArr + 36);
	std::vector<Vertex> vertices;
	cubeMesh.indicesSize = 36;
	cubeMesh.verticesSize = 36;

	for (int i = 0; i < indices.size(); i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = verticesArr[6*i];
		vector.y = verticesArr[6 * i + 1];
		vector.z = verticesArr[6 * i + 2];
		vertex.Position = vector;

		vector.x = verticesArr[6 * i + 3];
		vector.y = verticesArr[6 * i + 4];
		vector.z = verticesArr[6 * i + 5];
		vertex.Normal = vector;


		vertices.push_back(vertex);
	}


	cubeMesh.vertices = std::make_shared<Vertex[]>(vertices.size());
	cubeMesh.indices = std::make_shared<unsigned int[]>(indices.size());
	std::copy(vertices.begin(), vertices.end(), cubeMesh.vertices.get());
	std::copy(indices.begin(), indices.end(), cubeMesh.indices.get());

	glGenVertexArrays(1, &cubeMesh.VAO);
	glGenBuffers(1, &cubeMesh.VBO);
	glGenBuffers(1, &cubeMesh.EBO);
	glBindVertexArray(cubeMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, cubeMesh.verticesSize * sizeof(Vertex), cubeMesh.vertices.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeMesh.indicesSize * sizeof(unsigned int), cubeMesh.indices.get(), GL_STATIC_DRAW);
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

	cubeMesh.material.diffuse = diffuse;
	cubeMesh.material.specular = specular;
	cubeMesh.material.emmision = emmision;

	Model cubeModel;
	cubeModel.meshes.push_back(cubeMesh);
	return cubeModel;
}




Model buildSphereModel(float radius, int resolution, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emmision)
{
	glm::vec3 axises[6] = { glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -1.0) };
	Model sphereModel;
	for (int i = 0; i < 6; i++)
	{
		sphereModel.meshes.push_back(ConstructSphereFaceMesh(radius, resolution, axises[i], diffuse, specular, emmision));
	}
	return sphereModel;
}




Mesh ConstructSphereFaceMesh(float radius, int resolution, glm::vec3 localUp, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emmision)
{
	Mesh faceMesh;
	glm::vec3 axisA = glm::vec3(localUp.y, localUp.z, localUp.x);
	glm::vec3 axisB = glm::cross(localUp, axisA);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int i = 0;
	float maxX = 0;
	float minX = 2;
	float maxY = 0;
	float minY = 2;
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			Vertex vertex;
			glm::vec2 percent = glm::vec2(x, y) / (float)(resolution - 1);
			glm::vec3 position = localUp + (float)(percent.x - 0.5) * 2 * axisA + (float)(percent.y - 0.5) * 2 * axisB;
			position = pointOnCubeToPointOnSphere(position);
			vertex.Position = position * radius;
			vertex.Normal = normalize(position);
			float latitude = std::asin(position.y);
			float longtitude = std::atan2(position.x, position.z);

			float texX = (longtitude + glm::pi<float>()) / (2 * glm::pi<float>());
			float texY = (latitude / glm::pi<float>()) + 0.5;
			if (texX > maxX) maxX = texX;
			if (texY > maxY) maxY = texY;
			if (texX < minX) minX = texX;
			if (texY < minY) minY = texY;
			vertex.TexCoords = glm::vec2(texX, texY);
			vertices.push_back(vertex);
			if (x != resolution - 1 && y != resolution - 1)
			{
				indices.push_back(i);
				indices.push_back(i + resolution + 1);
				indices.push_back(i + resolution);

				indices.push_back(i);
				indices.push_back(i + 1);
				indices.push_back(i + resolution + 1);
			}
			i++;
		}
	}
	faceMesh.vertices = std::make_shared<Vertex[]>(vertices.size());
	faceMesh.indices = std::make_shared<unsigned int[]>(indices.size());
	std::copy(vertices.begin(), vertices.end(), faceMesh.vertices.get());
	std::copy(indices.begin(), indices.end(), faceMesh.indices.get());
	faceMesh.indicesSize = indices.size();
	faceMesh.verticesSize = vertices.size();

	glGenVertexArrays(1, &faceMesh.VAO);
	glGenBuffers(1, &faceMesh.VBO);
	glGenBuffers(1, &faceMesh.EBO);
	glBindVertexArray(faceMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, faceMesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, faceMesh.verticesSize * sizeof(Vertex), faceMesh.vertices.get(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceMesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceMesh.indicesSize * sizeof(unsigned int), faceMesh.indices.get(), GL_STATIC_DRAW);
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


	faceMesh.material.diffuse = diffuse;
	faceMesh.material.specular = specular;
	faceMesh.material.emmision = emmision;


	return faceMesh;
}


glm::vec3 pointOnCubeToPointOnSphere(glm::vec3 p)
{
	float xsq = p.x * p.x;
	float ysq = p.y * p.y;
	float zsq = p.z * p.z;

	float x = p.x * glm::sqrt(1 - (ysq + zsq) / 2 + (ysq * zsq) / 3);
	float y = p.y * glm::sqrt(1 - (zsq + xsq) / 2 + (zsq * xsq) / 3);
	float z = p.z * glm::sqrt(1 - (xsq + ysq) / 2 + (xsq * ysq) / 3);

	return glm::vec3(x, y, z);
}




