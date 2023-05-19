#include "RenderPipeline.h"
#include "Coordinator.h"


void RenderPipeline::Init(Shader shader, Shader instancedShader)
{
	m_Shader = shader;

	m_InstancedShader = instancedShader;

	m_CameraManager = Coordinator.RegisterSystem<CameraManager>();
	m_DirectionalLightsManager = Coordinator.RegisterSystem<DirectionalLightsManager>();
	m_PointLightsManager = Coordinator.RegisterSystem<PointLightsManager>();
	m_SpotLightsManager = Coordinator.RegisterSystem<SpotLightsManager>();
	m_FogSystem = Coordinator.RegisterSystem<FogSystem>();
	m_MeshRenderer = Coordinator.RegisterSystem<MeshRenderer>();
	m_InstancedMeshRenderer = Coordinator.RegisterSystem<InstancedMeshRenderer>();

	m_CameraManager->Init();
	m_DirectionalLightsManager->Init();
	m_PointLightsManager->Init();
	m_SpotLightsManager->Init();
	m_FogSystem->Init();
	m_MeshRenderer->Init();
	m_InstancedMeshRenderer->Init();


	m_Shaders.push_back(m_Shader);
	m_Shaders.push_back(m_InstancedShader);
	glEnable(GL_DEPTH_TEST);
}
void RenderPipeline::RenderFrame()
{

	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Shaders.push_back(skyBox.shader);
	m_CameraManager->OnUpdate(m_Shaders);
	m_Shaders.pop_back();
	m_DirectionalLightsManager->OnUpdate(m_Shaders);
	m_PointLightsManager->OnUpdate(m_Shaders);
	m_SpotLightsManager->OnUpdate(m_Shaders);
	m_FogSystem->OnUpdate(m_Shaders);
	m_Shader.Activate();
	m_MeshRenderer->OnUpdate(m_Shader);
	m_InstancedShader.Activate();
	m_InstancedMeshRenderer->OnUpdate(m_InstancedShader);
	skyBox.Render();
}

void RenderPipeline::ChangeShaders(Shader shader, Shader instancedShader)
{
	m_Shader = shader;
	m_InstancedShader = instancedShader;
	m_Shaders.clear();
	m_Shaders.push_back(m_Shader);
	m_Shaders.push_back(m_InstancedShader);
}