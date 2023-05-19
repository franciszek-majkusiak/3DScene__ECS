#pragma once
#include "ModelLoader/ModelLoader.h"



struct InstancedModel
{
	Model model;
	std::shared_ptr<glm::mat4[]> modelMatrices = nullptr;
	size_t amount = 0;
	GLuint matrixesVBO;


	InstancedModel(std::string path, std::vector<glm::mat4> matrices)
	{
		model = loadModelFromObj(path);
		amount = matrices.size();
		modelMatrices = std::make_shared<glm::mat4[]>(matrices.size());
		std::copy(matrices.begin(), matrices.end(), modelMatrices.get());

		glGenBuffers(1, &matrixesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, matrixesVBO);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices.get(), GL_STATIC_DRAW);
		for (Mesh& mesh : model.meshes)
		{
			glBindVertexArray(mesh.VAO);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindVertexArray(0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	InstancedModel(Model prefabModel, std::vector<glm::mat4> matrices)
	{
		model = prefabModel;
		amount = matrices.size();
		modelMatrices = std::make_shared<glm::mat4[]>(matrices.size());
		std::copy(matrices.begin(), matrices.end(), modelMatrices.get());

		glGenBuffers(1, &matrixesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, matrixesVBO);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices.get(), GL_STATIC_DRAW);
		for (Mesh& mesh : model.meshes)
		{
			glBindVertexArray(mesh.VAO);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindVertexArray(0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	InstancedModel() = default;
};
