#include "InstancedMeshRenderer.h"
#include "Coordinator.h"
#include "Components/InstancedModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include "RenderContext.h"


void InstancedMeshRenderer::Init()
{
	signature.set(Coordinator.GetComponentType<InstancedModel>());
	Coordinator.SetSystemSignature<InstancedMeshRenderer>(signature);
}
void InstancedMeshRenderer::OnUpdate(Shader shader)
{
	for (auto const& entity : Entities)
	{
		InstancedModel const& instancedModel = Coordinator.GetComponent<InstancedModel>(entity);


		for (auto const& mesh : instancedModel.model.meshes)
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
			glDrawElementsInstanced(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, 0, instancedModel.amount);
			glBindVertexArray(0);
		}
	}
}
