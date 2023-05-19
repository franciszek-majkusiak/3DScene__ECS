#include "Scene.h"
#include <random>

//#define DEFFERED

void Scene::Init()
{
	// Load models for multiple usage
	Model tree1Model = loadModelFromObj("Assets/Forest/Tree1.obj");
	AddPrefabModel(tree1Model);
	Model tree2Model = loadModelFromObj("Assets/Forest/Tree2.obj");
	AddPrefabModel(tree2Model);
	Model bush1Model = loadModelFromObj("Assets/Forest/Bush1.obj");
	AddPrefabModel(bush1Model);
	Model rock1Model = loadModelFromObj("Assets/Forest/Rock1.obj");
	AddPrefabModel(rock1Model);
	Model lightBugModel = buildSphereModel(0.3, 5, glm::vec3(1), glm::vec3(1), glm::vec3(1));
	AddPrefabModel(lightBugModel);
	Model carModel = loadModelFromObj("Assets/Car/car.obj");
	AddPrefabModel(carModel);


	Coordinator.Init();
	// Registering Components
	Coordinator.RegisterComponent<Transform>();
	Coordinator.RegisterComponent<Camera>();
	Coordinator.RegisterComponent<MainCamera>();
	Coordinator.RegisterComponent<Mesh>();
	Coordinator.RegisterComponent<Model>();
	Coordinator.RegisterComponent<PointLight>();
	Coordinator.RegisterComponent<SpotLight>();
	Coordinator.RegisterComponent<DirectionalLight>();
	Coordinator.RegisterComponent<InstancedModel>();
	Coordinator.RegisterComponent<Fog>();
	Coordinator.RegisterComponent<Vibration>();


	// Shaders
	// Phong
	phong_shader =Shader("src/Shaders/phong.vert", "src/Shaders/phong.frag");
	instancedPhong_shader = Shader("src/Shaders/instanced.vert", "src/Shaders/phong.frag");

	//Gouraud
	gouraud_shader =Shader("src/Shaders/gouraud.vert", "src/Shaders/gouraud.frag");
	instancedGouraud_shader = Shader("src/Shaders/instancedgouraud.vert", "src/Shaders/gouraud.frag");

	shaders[0] = phong_shader;
	shaders[1] = gouraud_shader;
	instancedShaders[0] = instancedPhong_shader;
	instancedShaders[1] = instancedGouraud_shader;

	// Deffered shading
	geometryPass_shader = Shader("src/Shaders/phong.vert", "src/Shaders/geometrypass.frag");
	instancedGeometryPass_shader = Shader("src/Shaders/instanced.vert", "src/Shaders/geometrypass.frag");
	lightingPass_shader = Shader("src/Shaders/lightingpass.vert", "src/Shaders/lightingpass.frag");
#ifdef DEFFERED
	defferedShadingPipeline.Init(geometryPass_shader, lightingPass_shader, instancedGeometryPass_shader);
#endif
#ifndef DEFFERED
	renderPipeline.Init(phong_shader, instancedPhong_shader);
#endif

	// Skybox
	std::vector<std::string> skyBoxFaces({ "Assets/SkyBox/bluecloud_rt.jpg", "Assets/SkyBox/bluecloud_lf.jpg" ,"Assets/SkyBox/bluecloud_up.jpg" ,"Assets/SkyBox/bluecloud_dn.jpg" ,"Assets/SkyBox/bluecloud_bk.jpg" ,"Assets/SkyBox/bluecloud_ft.jpg" });
	SkyBox cloudySkyBox(skyBoxFaces);
	renderPipeline.skyBox = cloudySkyBox;
	// Systems
	modelsCleanUp = Coordinator.RegisterSystem<ModelsCleanUp>();
	modelsCleanUp->Init();

	instancedModelsCleanUp = Coordinator.RegisterSystem<InstancedModelsCleanUp>();
	instancedModelsCleanUp->Init();

	sunSystem = Coordinator.RegisterSystem<SunSystem>();
	sunSystem->Init();


	// Game Objects
	GameObject& editorCamera = sceneGraph.CreateGameObject("Editor camera");
	editorCamera.transform.Translate(glm::vec3(0.0f, 100.0f, 0.0f));
	editorCamera.transform.Rotate(glm::vec3(0.0f, 225.0f, 0.0f));
	editorCamera.AddComponent<Camera>(Camera());
	editorCamera.AddComponent<MainCamera>(MainCamera());
	editorCamera.AddScript(new EditorCameraMovement());
	cameras.push_back(&editorCamera);
	activeCamera = &editorCamera;


	GameObject& player = sceneGraph.CreateGameObject("Car");
	player.transform.Rotate(-135.0f, glm::vec3(0, 1, 0));
	player.transform.Translate(glm::vec3(50.0f, 0.0f, 50.0f));
	player.transform.Scale(glm::vec3(2.0f));
	player.AddComponent<Model>(carModel);
	CarMovementController* movement = new CarMovementController();
	player.AddScript(movement);
	player.AddComponent<Vibration>(Vibration());
	player.AddScript(new VibrationsScript());


	// Car 3rd person camera
	GameObject *carCamera = new GameObject("Car camera");
	carCamera->transform.Translate(glm::vec3(0.0f, 10.0f, 14.0f));
	carCamera->transform.Rotate(-15, carCamera->transform.Right());
	carCamera->AddComponent<Camera>(Camera());
	//camera->AddComponent<MainCamera>(MainCamera());
	movement->camera = carCamera;

	cameras.push_back(carCamera);

	//Car Lights
	GameObject* leftCarLight = new GameObject("Left car light");
	leftCarLight->transform.Translate(glm::vec3(-1.0f, 0.94f, -2.4f));
	leftCarLight->AddComponent<SpotLight>(SpotLight());

	GameObject* rightCarLight = new GameObject("Right car light");
	rightCarLight->transform.Translate(glm::vec3(1.0f, 0.94f, -2.4f));
	rightCarLight->AddComponent<SpotLight>(SpotLight());

	player.AddChild(leftCarLight);
	player.AddChild(rightCarLight);
	player.AddChild(carCamera);



	// Sun
	ecs::Entity sun = Coordinator.CreateEntity();
	Coordinator.AddComponent<DirectionalLight>(sun, DirectionalLight());

	// Fog
	GameObject& fog = sceneGraph.CreateGameObject("Fog", false);
	Fog fogProperties = Fog();
	fogProperties.isEnabled = true;
	fogProperties.equation = 2;
	fogProperties.density = 0.005;
	fog.AddComponent<Fog>(fogProperties);



	// Terrain
	std::random_device rd;
	std::mt19937 mt(rd());
	float terrainSize = 500.0f;
	std::uniform_real_distribution<float> dist(0, terrainSize);
	ecs::Entity terrain = Coordinator.CreateEntity();
	Model terrainModel = buildTerrainMesh(terrainSize, terrainSize, 100, "", /*assets/terrain_normalMap.png*/"");
	Coordinator.AddComponent<Model>(terrain, terrainModel);
	Coordinator.AddComponent<Transform>(terrain, Transform());


	// Top Camera
	GameObject& followingCamera = sceneGraph.CreateGameObject("Following camera");
	followingCamera.transform.Translate(glm::vec3(terrainSize/2.0f, 200.0f, terrainSize + 50.0f));
	followingCamera.AddComponent<Camera>(Camera());
	ObserverScript* observerScript = new ObserverScript();
	observerScript->target = &player;
	followingCamera.AddScript(observerScript);
	cameras.push_back(&followingCamera);



	int numOfInstances = 200;

	std::vector<glm::mat4> tree1ModelMatrixes;
	for (int i = 0; i < numOfInstances; i++)
	{
		glm::mat4 modelMat = glm::mat4(1);
		modelMat = glm::translate(modelMat, glm::vec3(dist(mt), 0, dist(mt)));
		modelMat = glm::rotate(modelMat, dist(mt), glm::vec3(0, 1, 0));
		tree1ModelMatrixes.push_back(modelMat);
	}
	InstancedModel instancedtree1Model = InstancedModel(tree1Model, tree1ModelMatrixes);
	ecs::Entity tree1 = Coordinator.CreateEntity();
	Coordinator.AddComponent<InstancedModel>(tree1, instancedtree1Model);

	std::vector<glm::mat4> tree2ModelMatrixes;
	for (int i = 0; i < numOfInstances; i++)
	{
		glm::mat4 modelMat = glm::mat4(1);
		modelMat = glm::translate(modelMat, glm::vec3(dist(mt), 0, dist(mt)));
		tree2ModelMatrixes.push_back(modelMat);
	}
	InstancedModel instancedtree2Model = InstancedModel(tree2Model, tree2ModelMatrixes);
	ecs::Entity tree2 = Coordinator.CreateEntity();
	Coordinator.AddComponent<InstancedModel>(tree2, instancedtree2Model);

	std::vector<glm::mat4> rockModelMatrixes;
	for (int i = 0; i < numOfInstances; i++)
	{
		glm::mat4 modelMat = glm::mat4(1);
		modelMat = glm::translate(modelMat, glm::vec3(dist(mt), 0, dist(mt)));
		modelMat = glm::rotate(modelMat, dist(mt), glm::vec3(0, 1, 0));
		rockModelMatrixes.push_back(modelMat);
	}
	InstancedModel instancedrockModel = InstancedModel(rock1Model, rockModelMatrixes);
	ecs::Entity rock = Coordinator.CreateEntity();
	Coordinator.AddComponent<InstancedModel>(rock, instancedrockModel);

	std::vector<glm::mat4> bushModelMatrixes;
	for (int i = 0; i < numOfInstances; i++)
	{
		glm::mat4 modelMat = glm::mat4(1);
		modelMat = glm::translate(modelMat, glm::vec3(dist(mt), 0, dist(mt)));
		modelMat = glm::rotate(modelMat, dist(mt), glm::vec3(0, 1, 0));
		bushModelMatrixes.push_back(modelMat);
	}
	InstancedModel instancedbushModel = InstancedModel(bush1Model, bushModelMatrixes);
	ecs::Entity bush = Coordinator.CreateEntity();
	Coordinator.AddComponent<InstancedModel>(bush, instancedbushModel);


	for (int i = 0; i < 10; i++)
	{
		ecs::Entity pointLight = Coordinator.CreateEntity();
		Transform pLTransform;
		pLTransform.Translate(glm::vec3(dist(mt), (float)rand() / (float)RAND_MAX * 10.0f, dist(mt)));
		Coordinator.AddComponent<Transform>(pointLight, pLTransform);
		PointLight pL;
		pL.UpdateRadius();
		Coordinator.AddComponent<PointLight>(pointLight, pL);
		Coordinator.AddComponent<Model>(pointLight, lightBugModel);


		ecs::Entity testSphere = Coordinator.CreateEntity();
		Transform spTransform;
		spTransform.Translate(pLTransform.position + glm::vec3(3.0f));
		Coordinator.AddComponent<Transform>(testSphere, spTransform);
		Coordinator.AddComponent<Model>(testSphere, buildSphereModel(1, 10, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0)));
	}


}

void Scene::StartUp()
{
	sunSystem->OnStartUp(10.0f);
	sceneGraph.OnStartUp();
}

void Scene::Update(float deltaTime)
{
	sceneGraph.OnUpdate(deltaTime);
	sceneGraph.OnLateUpdate(deltaTime);
	sunSystem->OnUpdate(deltaTime);
#ifdef DEFFERED
	defferedShadingPipeline.RenderFrame();
#endif
#ifndef DEFFERED
	renderPipeline.RenderFrame();
#endif
}
void Scene::RenderGui()
{
	const char* currentItem;
#ifndef DEFFERED
	currentItem = shadingTypes[activeShaderOption];
	if (ImGui::BeginCombo("Shading option", currentItem))
	{
		for (int i = 0; i < 2; i++)
		{
			bool is_selected = (currentItem == shadingTypes[i]);
			if (ImGui::Selectable(shadingTypes[i], is_selected))
			{
				currentItem = shadingTypes[i];
				activeShaderOption = i;
				renderPipeline.ChangeShaders(shaders[i], instancedShaders[i]);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
#endif


	currentItem = activeCamera->tag.c_str();
	if (ImGui::BeginCombo("Active camera", currentItem))
	{
		activeCamera->RemoveComponent<MainCamera>();
		for (int i = 0; i < cameras.size(); i++)
		{
			bool is_selected = (currentItem == cameras[i]->tag.c_str());
			if (ImGui::Selectable(cameras[i]->tag.c_str(), is_selected))
			{
				currentItem = cameras[i]->tag.c_str();
				activeCamera = cameras[i];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		activeCamera->AddComponent<MainCamera>(MainCamera());

		ImGui::EndCombo();
	}

	sceneGraph.OnGuiRender();
}


void Scene::End()
{
	/*modelsCleanUp->OnEnd();
	instancedModelsCleanUp->OnEnd();
	for (Model& model : prefabs)
	{
		for (Mesh& mesh : model.meshes)
		{
			delete[] mesh.indices;
			delete[] mesh.vertices;
		}
	}*/
}
