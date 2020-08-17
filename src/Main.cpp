#include <WSEngine.h>

using namespace std;
using namespace glm;

unsigned int width = 1280;
unsigned int height = 720;

struct UiPeremeters
{
	bool show_demo_window;

	bool useDepthTest;
	bool useHDR;
	bool useMSAA;
	bool cullback;
	bool oldDepthTest;
	bool oldHDR;
	bool oldMSAA;
	bool oldcullback;

	bool useToneMapping;
	float autoExplosion;
	bool useVignette;
	float bloomThreshold;
	RenderTexture *rt;

	Transform *modelTrans;
	Light *light;
	vec4 ambientcolor;
	EmiMaterial* emissionmat;
};
UiPeremeters gPeremeters;

void uicallback()
{

	if (gPeremeters.show_demo_window)
		ImGui::ShowDemoWindow(&gPeremeters.show_demo_window);

	ImGui::Begin("WSEngine Ui!");

	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::BulletText("Press Alt and move Left Button to rotate view\n");
		ImGui::BulletText("Roll the scroll wheel to zoom view\n");
		ImGui::BulletText("Move Right Button to move view");
		ImGui::Separator();
	}

	ImGui::Checkbox("Demo Window", &gPeremeters.show_demo_window);

	if (ImGui::CollapsingHeader("Render Setting"))
	{
		ImGui::Checkbox("Use Depth Test", &gPeremeters.useDepthTest);
		if (gPeremeters.useDepthTest != gPeremeters.oldDepthTest)
		{
			RenderManager::Instance().SetDepthTest(gPeremeters.useDepthTest);
			gPeremeters.oldDepthTest = gPeremeters.useDepthTest;
		}
		ImGui::Checkbox("Use HDR", &gPeremeters.useHDR);
		if (gPeremeters.useHDR != gPeremeters.oldHDR)
		{
			RenderManager::Instance().SetHDR(gPeremeters.useHDR);
			gPeremeters.oldHDR = gPeremeters.useHDR;
		}
		ImGui::Checkbox("Use MSAA", &gPeremeters.useMSAA);
		if (gPeremeters.useMSAA != gPeremeters.oldMSAA)
		{
			RenderManager::Instance().SetMSAA(gPeremeters.useMSAA);
			gPeremeters.oldMSAA = gPeremeters.useMSAA;
		}
		ImGui::Checkbox("Cull Back", &gPeremeters.cullback);
		RenderManager::Instance().SetCullBack(gPeremeters.cullback);
	}

	if (ImGui::CollapsingHeader("Postprocess Setting"))
	{
		ImGui::Checkbox("Use Vignette", &gPeremeters.useVignette);
		RenderManager::Instance().SetVignette(gPeremeters.useVignette);

		ImGui::Checkbox("Use ToneMapping", &gPeremeters.useToneMapping);
		RenderManager::Instance().SetToneMapping(gPeremeters.useToneMapping);
		ImGui::SliderFloat("Auto Explosioin", (float *)&gPeremeters.autoExplosion, 0.1f, 5.0f);
		RenderManager::Instance().SetAutoExplosion(gPeremeters.autoExplosion);

		ImGui::SliderFloat("Bloom Threshold", (float *)&gPeremeters.bloomThreshold, 0.1f, 3.0f);
		RenderManager::Instance().SetThreshold(gPeremeters.bloomThreshold);
	}

	ImGui::Text("tmp RT");
	ImGui::Image((void *)(intptr_t)(gPeremeters.rt->textureColorbuffer), ImVec2(256, 180), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Light Setting"))
	{
		ImGui::ColorEdit4("ambient color", (float *)&gPeremeters.ambientcolor);
		RenderManager::Instance().SetAmbientColor(gPeremeters.ambientcolor);
		ImGui::ColorEdit4("light color", (float *)&gPeremeters.light->lightColor);
		ImGui::SliderFloat("light intensity", (float *)&gPeremeters.light->lightIntensity, 0.0f, 5.0f);
		ImGui::SliderFloat("emission intensity", (float *)&gPeremeters.emissionmat->intensity, 0.0f, 5.0f);
	}

	if (ImGui::CollapsingHeader("Model Transform"))
	{
		ImGui::SliderFloat3("Position", (float *)&gPeremeters.modelTrans->position, -2.0f, 2.0f);
		ImGui::SliderFloat3("Rotation", (float *)&gPeremeters.modelTrans->rotation, -180.0f, 180.0f);
		ImGui::SliderFloat3("Scale", (float *)&gPeremeters.modelTrans->scale, 0.0f, 2.0f);
	}

	float fps = FPS::Instance().GetFps();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / fps, fps);

	ImGui::End();
}

int main()
{
	WSEngine engine;
	if (!engine.Init())
	{
		return -1;
	}
	cout << "complete init" << endl;

	//resource loading
	vector<string> outnames;
	vector<Mesh *> meshs = ResourcesManager::Instance().LoadMesh("../../assets/house.fbx", &outnames);
	vector<Mesh *> lightmeshs = ResourcesManager::Instance().LoadMesh("../../assets/house_light.fbx", &outnames);
	ShaderClass *shader = ResourcesManager::Instance().LoadShader("../../assets/pbrVert.txt", "../../assets/pbrFrag.txt", "pbr");
	PBRMaterial *mat = ResourcesManager::Instance().CreateMaterial<PBRMaterial>(shader, "pbrmat");
	// Texture *mask = ResourcesManager::Instance().LoadTexture("../../assets/mask.tga", "mask");
	// mat->textures.push_back(mask);

	vector<string> emissionoutnames;
	vector<Mesh *> emissionmeshs = ResourcesManager::Instance().LoadMesh("../../assets/house_light.fbx", &emissionoutnames);
	ShaderClass *emissionshader = ResourcesManager::Instance().LoadShader("../../assets/emissionVert.txt", "../../assets/emissionFrag.txt", "emission");
	EmiMaterial *emissionmat = ResourcesManager::Instance().CreateMaterial<EmiMaterial>(emissionshader, "emissionmat");
	emissionmat->intensity = 2.5f;

	//mesh entity
	Entity *monkey = EntityManager::Instance().CreateEntity();
	MeshRender *meshRender = monkey->AddComponent<MeshRender>();
	meshRender->SetMaterial(mat);
	meshRender->SetMesh(meshs[0]);

	//emission mesh entity
	Entity *emissionEntity = EntityManager::Instance().CreateEntity();
	MeshRender *emissionmeshRender = emissionEntity->AddComponent<MeshRender>();
	emissionmeshRender->SetMaterial(emissionmat);
	emissionmeshRender->SetMesh(emissionmeshs[0]);

	//light entity
	Entity *sun = EntityManager::Instance().CreateEntity();
	Light *lightcom = sun->AddComponent<Light>();
	lightcom->lightIntensity = 0.8;

	//gperameters setting
	gPeremeters.show_demo_window = false;
	gPeremeters.useDepthTest = true;
	gPeremeters.useHDR = true;
	gPeremeters.useMSAA = true;
	gPeremeters.oldDepthTest = true;
	gPeremeters.oldHDR = true;
	gPeremeters.oldMSAA = true;
	gPeremeters.cullback = false;

	gPeremeters.useToneMapping = true;
	gPeremeters.useVignette = true;
	gPeremeters.bloomThreshold = 1.0f;

	gPeremeters.ambientcolor = vec4(0.45, 0.5, 0.5, 1.0);
	gPeremeters.autoExplosion = 1.0f;
	gPeremeters.modelTrans = monkey->GetComponent<Transform>();
	gPeremeters.light = sun->GetComponent<Light>();
	gPeremeters.rt = RenderManager::Instance().GetRenderTexture();
	gPeremeters.emissionmat = emissionmat;

	engine.Run(uicallback);
	cout << "complete run" << endl;

	engine.Quit();
	cout << "complete quit" << endl;

	return 0;
}