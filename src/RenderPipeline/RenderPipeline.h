#pragma once
#include "CameraManager.h"
#include "DirectionalLightsManager.h"
#include "PointLightsManager.h"
#include "SpotLightsManager.h"
#include "MeshRenderer.h"
#include "InstancedMeshRenderer.h"
#include "FogSystem.h"
#include <vector>
#include "SkyBox.h"



class RenderPipeline
{
public:
	void Init(Shader shader, Shader instancedShader);
	void RenderFrame();

	void ChangeShaders(Shader shader, Shader instancedShader);


	SkyBox skyBox;
private:
	Shader m_Shader;
	Shader m_InstancedShader;
	std::shared_ptr<CameraManager> m_CameraManager;
	std::shared_ptr<DirectionalLightsManager> m_DirectionalLightsManager;
	std::shared_ptr<PointLightsManager> m_PointLightsManager;
	std::shared_ptr<SpotLightsManager> m_SpotLightsManager;
	std::shared_ptr<FogSystem> m_FogSystem;
	std::shared_ptr<MeshRenderer> m_MeshRenderer;
	std::shared_ptr<InstancedMeshRenderer> m_InstancedMeshRenderer;
	std::vector<Shader> m_Shaders;
};
