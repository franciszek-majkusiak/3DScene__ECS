#pragma once
#include "RenderPipeline/RenderPipeline.h"
#include "RenderPipeline/DefferedShadingPipeline.h"
#include "Components/Components.h"
#include "Scripts/Scripts.h"
#include "SceneGraph.h"
#include "Systems/Systems.h"

class Scene
{
public:
	void Init();
	void StartUp();
	void Update(float deltaTime);
	void RenderGui();
	void End();


private:
	void AddPrefabModel(Model& model) 
	{ 
		model.isPrefab = true; 
		prefabs.push_back(model); 
	}
	SceneGraph sceneGraph;
	RenderPipeline renderPipeline;
	DefferedShadingPipeline defferedShadingPipeline;
	std::vector<GameObject*> cameras;
	GameObject* activeCamera;
	Shader phong_shader;
	Shader instancedPhong_shader;
	Shader gouraud_shader;
	Shader instancedGouraud_shader;
	Shader geometryPass_shader;
	Shader instancedGeometryPass_shader;
	Shader lightingPass_shader;
	std::shared_ptr<MovementScript> movementScript;
	std::shared_ptr<ModelsCleanUp> modelsCleanUp;
	std::shared_ptr<InstancedModelsCleanUp> instancedModelsCleanUp;
	std::shared_ptr<SunSystem> sunSystem;

	std::vector<Model> prefabs;
	std::vector<InstancedModel> instancedModels;

	int activeShaderOption = 0;
	const char* shadingTypes[2] = { "Phong shading", "Gouraud shading" };
	Shader shaders[2];
	Shader instancedShaders[2];
};
