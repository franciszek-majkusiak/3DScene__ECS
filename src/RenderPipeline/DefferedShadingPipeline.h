#pragma once
#include "CameraManager.h"
#include "PointLightsManager.h"
#include "SpotLightsManager.h"
#include "GeometryPass.h"
#include "LightingPass.h"
#include "DirectionalLightsManager.h"
#include "InstancedGeometryPass.h"
#include "FogSystem.h"



class DefferedShadingPipeline
{
public:
	void Init(Shader shaderGeometry, Shader shaderLighting, Shader instancedShaderGeometry);
	void RenderFrame();

private:
	Shader m_ShaderGeometryPass;
	Shader m_ShaderInstancedGeometryPass;
	Shader m_ShaderLightingPass;
	std::shared_ptr<CameraManager> m_CameraManager;
	std::shared_ptr<DirectionalLightsManager> m_DirectionalLightsManager;
	std::shared_ptr<PointLightsManager> m_PointLightsManager;
	std::shared_ptr<SpotLightsManager> m_SpotLightsManager;
	std::shared_ptr<FogSystem> m_FogSystem;
	std::shared_ptr<GeometryPass> m_GeometryPass;
	std::shared_ptr<InstancedGeometryPass> m_InstancedGeometryPass;
	std::shared_ptr<LightingPass> m_LightingPass;
	std::vector<Shader> m_ShadersForCameraPass;
	std::vector<Shader> m_ShadersForLightsPass;


	int m_Width;
	int m_Height;

	GLuint m_gBuffer;
	GLuint m_gPosition;
	GLuint m_gNormal;
	GLuint m_gDiffuseSpecular;
	GLuint m_gAmbientShininess;
	GLuint m_Depth;

	GLuint m_Attachments[4];
};
