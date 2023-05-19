#include "DefferedShadingPipeline.h"
#include "Coordinator.h"
#include "Input/Input.h"


void DefferedShadingPipeline::Init(Shader shaderGeometry, Shader shaderLighting, Shader instancedShaderGeometry)
{
    m_ShaderGeometryPass = shaderGeometry;
    m_ShaderLightingPass = shaderLighting;
    m_ShaderInstancedGeometryPass = instancedShaderGeometry;

    m_Width = Input::GetWindowWidth();
    m_Height = Input::GetWindowHeight();

    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    // position color buffer
    glGenTextures(1, &m_gPosition);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);
    // normal color buffer
    glGenTextures(1, &m_gNormal);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
    // diffuse + ambient color buffer
    glGenTextures(1, &m_gDiffuseSpecular);
    glBindTexture(GL_TEXTURE_2D, m_gDiffuseSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gDiffuseSpecular, 0);
    // specular + shininess color buffer
    glGenTextures(1, &m_gAmbientShininess);
    glBindTexture(GL_TEXTURE_2D, m_gAmbientShininess);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gAmbientShininess, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    m_Attachments[0] = GL_COLOR_ATTACHMENT0;
    m_Attachments[1] = GL_COLOR_ATTACHMENT1;
    m_Attachments[2] = GL_COLOR_ATTACHMENT2;
    m_Attachments[3] = GL_COLOR_ATTACHMENT3;
    glDrawBuffers(4, m_Attachments);

    glGenRenderbuffers(1, &m_Depth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Depth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);


    m_ShaderLightingPass.Activate();
    m_ShaderLightingPass.setInt("gPosition", 0);
    m_ShaderLightingPass.setInt("gNormal", 1);
    m_ShaderLightingPass.setInt("gDiffuseSpecular", 2);
    m_ShaderLightingPass.setInt("gAmbientShininess", 3);





    // Init Systems
    m_CameraManager = Coordinator.RegisterSystem<CameraManager>();
    m_DirectionalLightsManager = Coordinator.RegisterSystem<DirectionalLightsManager>();
    m_PointLightsManager = Coordinator.RegisterSystem<PointLightsManager>();
    m_SpotLightsManager = Coordinator.RegisterSystem<SpotLightsManager>();
    m_FogSystem = Coordinator.RegisterSystem<FogSystem>();
    m_GeometryPass = Coordinator.RegisterSystem<GeometryPass>();
    m_InstancedGeometryPass = Coordinator.RegisterSystem<InstancedGeometryPass>();
    m_LightingPass = Coordinator.RegisterSystem<LightingPass>();

    m_CameraManager->Init();
    m_DirectionalLightsManager->Init();
    m_PointLightsManager->Init();
    m_SpotLightsManager->Init();
    m_FogSystem->Init();
    m_GeometryPass->Init();
    m_InstancedGeometryPass->Init();
    m_LightingPass->Init();



    m_ShadersForCameraPass.push_back(m_ShaderGeometryPass);
    m_ShadersForCameraPass.push_back(m_ShaderInstancedGeometryPass);

    m_ShadersForLightsPass.push_back(m_ShaderLightingPass);

    glEnable(GL_DEPTH_TEST);

}



void DefferedShadingPipeline::RenderFrame()
{
    if (Input::GetWindowWidth() != m_Width || Input::GetWindowHeight() != m_Height)
    {
        m_Width = Input::GetWindowWidth();
        m_Height = Input::GetWindowHeight();

        glBindTexture(GL_TEXTURE_2D, m_gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, m_gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, m_gDiffuseSpecular);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, m_gAmbientShininess);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindRenderbuffer(GL_RENDERBUFFER, m_Depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Geometry Pass
	glm::vec3 cameraPosition = m_CameraManager->OnUpdate(m_ShadersForCameraPass);
    m_ShaderGeometryPass.Activate();
    m_GeometryPass->OnUpdate(m_ShaderGeometryPass);
    m_ShaderInstancedGeometryPass.Activate();
    m_InstancedGeometryPass->OnUpdate(m_ShaderInstancedGeometryPass);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Lighting Pass
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_ShaderLightingPass.Activate();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_gDiffuseSpecular);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_gAmbientShininess);

    m_ShaderLightingPass.setVec3("viewPos", cameraPosition);
    m_DirectionalLightsManager->OnUpdate(m_ShadersForLightsPass);
	m_PointLightsManager->OnUpdate(m_ShadersForLightsPass);
    m_SpotLightsManager->OnUpdate(m_ShadersForLightsPass);
    m_FogSystem->OnUpdate(m_ShadersForLightsPass);
    m_LightingPass->OnUpdate(m_ShaderLightingPass);


    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}