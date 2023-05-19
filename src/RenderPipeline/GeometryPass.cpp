#include "GeometryPass.h"
#include "Coordinator.h"
#include "Components/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include "RenderContext.h"
#include <GLFW/glfw3.h>


void GeometryPass::Init()
{
	signature.set(Coordinator.GetComponentType<Transform>());
	signature.set(Coordinator.GetComponentType<Model>());
	Coordinator.SetSystemSignature<GeometryPass>(signature);
}
void GeometryPass::OnUpdate(Shader shader)
{
	for (auto const& entity : Entities)
	{
		auto const& model = Coordinator.GetComponent<Model>(entity);
		auto& transform = Coordinator.GetComponent<Transform>(entity);

		glm::mat4 modelMatrix = transform.modelMatrix;
		glm::mat4 normalMatrix = transform.normalMatrix;

		//shader->Activate();
		shader.setMat4("model", modelMatrix);
		shader.setMat4("normalMat", normalMatrix);

		for (auto const& mesh : model.meshes)
		{
			shader.setVec3("material.ambient", mesh.material.ambient);
			shader.setVec3("material.diffuse", mesh.material.diffuse);
			shader.setVec3("material.specular", mesh.material.specular);
			shader.setVec3("material.emmision", mesh.material.emmision);

			int i = 0;
			if (mesh.material.isUsingDiffuseTexture)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh.material.texture_diffuse.ID);
				std::string name = mesh.material.texture_diffuse.type;
				shader.setInt(("material." + name).c_str(), i);
				shader.setBool("material.isUsingDiffuseTexture", true);
				i++;
			}
			else
				shader.setBool("material.isUsingDiffuseTexture", false);
			if (mesh.material.isUsingSpecularTexture)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh.material.texture_specular.ID);
				std::string name = mesh.material.texture_specular.type;
				shader.setInt(("material." + name).c_str(), i);
				shader.setBool("material.isUsingSpecularTexture", true);
				i++;
			}
			else
				shader.setBool("material.isUsingSpecularTexture", false);
			if (mesh.material.isUsingNormalsTexture)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh.material.texture_normals.ID);
				std::string name = mesh.material.texture_normals.type;
				shader.setInt(("material." + name).c_str(), i);
				shader.setBool("material.isUsingNormalsTexture", true);
				i++;
			}
			else
				shader.setBool("material.isUsingNormalsTexture", false);
			shader.setFloat("material.shininess", mesh.material.shininess);
			// draw mesh

			glBindVertexArray(mesh.VAO);
			glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}
